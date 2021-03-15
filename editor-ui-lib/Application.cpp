#include "stdafx.h"
#include "../render-lib/puha-render.h"
#include "Application.h"
#include "../curve-lib/curviness.h"
#include "../editor-lib/editor_math.h"
#include "../editor-lib/command_split.h"
#include "../editor-lib/command_add_curve.h"
#include "./tool_split.h"
#include "./tool_edit.h"
#include "./tool_param_count.h"
#include "../editor-lib/code_generator.h"

static void init_curve(curve& c);

void Application::ThreadMethod()
{
	try
	{
		// init code
		editor.document.curve_list.emplace_back();
		curve_to_screen = transformation{ 50, -100, 160, 100 };
		screen_to_curve = curve_to_screen.inverse();

		// main
		std::unique_lock<std::mutex> lock(thread_mutex);
		while (is_running) {
			auto did_work = DoWork();
			if (!did_work) {
				signal.wait(lock);
			}
		}
	}
	catch (std::exception exception)
	{
		is_running = false;
		is_error = true;
		error_message = exception.what();
	}
	catch (const char* message) 
	{
		is_running = false;
		is_error = true;
		error_message = message;
	}
}

bool Application::DoWork()
{
	execute_work_items();

	bool mouse_l_pressed = mouse_l && !mouse_l_prev;
	bool mouse_l_released = !mouse_l && mouse_l_prev;
	// Update Logic /////////////////////////

	// input segment_mouseover
	mouse_curve_x = screen_to_curve.apply_x(mouse_x);
	mouse_curve_y = screen_to_curve.apply_y(mouse_y);
	auto delta_x = mouse_x - last_mouse_x;
	auto delta_y = mouse_y - last_mouse_y;
	float curve_dx = screen_to_curve.scale_x * delta_x;
	float curve_dy = screen_to_curve.scale_y * delta_y;
	bool did_move = (delta_x != 0 || delta_y != 0);

	if (mouse_l_pressed) {
		mouse_press_x = mouse_x;
		mouse_press_y = mouse_y;
	}

	if (preview_command) {
		preview_command->undo();	
		preview_command.reset();
	}

	if (tool_instance) {
		tool_instance->transform_change(curve_to_screen);
		if (did_move || mouse_l_pressed || mouse_l_released) {
			tool_instance->update_mouse_curve(
				mouse_curve_x, mouse_curve_y);
			tool_instance->update_mouse_screen(
				mouse_x, mouse_y);
		}
		if (mouse_l_pressed) {
			tool_instance->mouse_l_press();
		}
		if (mouse_l_released) {
			tool_instance->mouse_l_release();
		}
		if (tool_instance->is_complete()) {
			auto cmd = tool_instance->get_command();
			if (cmd) {
				editor.history.commit(std::move(cmd));
			}
			tool_instance = new_tool_instance();
		}
		else {
			auto cmd = tool_instance->get_command();
			if (cmd) {
				preview_command = std::move(cmd);
				preview_command->exec();
			}
		}
	}
	else {
		if (mouse_l) {
			curve_to_screen.translate_x += delta_x;
			curve_to_screen.translate_y += delta_y;
			screen_to_curve = curve_to_screen.inverse();
		}
		if (mouse_l_released
			&& mouse_press_x == mouse_x
			&& mouse_press_y == mouse_y) {
			select_nearest_curve(mouse_x, mouse_y);
		}
	}

	// consume state
	last_mouse_x = mouse_x;
	last_mouse_y = mouse_y;
	mouse_l_prev = mouse_l;

	this->DoRenderingWork();

	return false;
}

void Application::defer(std::function<void()> fn)
{
	work_items_mutex.lock();
	work_items.push(fn);
	work_items_mutex.unlock();
	signal.notify_all();
}

void Application::execute_work_items()
{
	work_items_mutex.lock();
	while (!work_items.empty())
	{
		auto fn = work_items.front();
		work_items.pop();
		work_items_mutex.unlock();
		fn();
		work_items_mutex.lock();
	}
	work_items_mutex.unlock();
}

void Application::select_nearest_curve(int x, int y)
{
	const float max_select_threshold_pixels = 32;
	size_t best_index = -1;
	auto best_distance = max_select_threshold_pixels;
	for (size_t i = 0; i < editor.document.curve_list.size(); i++) {
		auto curve_x = screen_to_curve.apply_x(x);
		auto curve_y = screen_to_curve.apply_y(y);
		auto curve_y_value = editor.document.curve_list[i].eval(curve_x);
		auto curve_distance_y = curve_y_value - curve_y;
		auto screen_distance = abs(curve_to_screen.apply_scaling_y(curve_distance_y));
		if (i == the_curve_editor.curve_index) {
			screen_distance -= 1; // bias towards selected
		}
		if (screen_distance < best_distance) {
			best_distance = screen_distance;
			best_index = i;
		}
	}
	the_curve_editor.curve_index = best_index;
}

void Application::active_tool_factory(std::function<std::unique_ptr<tool_base>()> factory)
{
	tool_factory = factory;
	tool_instance = new_tool_instance();
}

std::unique_ptr<tool_base> Application::new_tool_instance()
{
	auto instance = tool_factory();
	update_all_tool_props(*instance);
	return instance;
}

void Application::update_all_tool_props(tool_base& tool)
{
	tool.update_mouse_curve(
		mouse_curve_x, mouse_curve_y);
	tool.update_mouse_screen(
		mouse_x, mouse_y);
}

void Application::DoRenderingWork()
{
	const int screen_width = gfx.Width;
	const int screen_height = gfx.Height;

	// Redraw ///////////////////////////////

	gfx.SetColor(0);
	gfx.RectangleFilled(0, 0, 319, 199);
	gfx.SetColor(0x222222);

	render.curve_to_screen = curve_to_screen;
	render.screen_to_curve = screen_to_curve;

	auto curve_index = the_curve_editor.curve_index;
	if (curve_index >= 0 
		&& curve_index < editor.document.curve_list.size()) {
		auto& selected_curve = editor.document.curve_list[curve_index];
		render.render_separator_lines(selected_curve, 0x222222);
	}

	int colors[] = { 0x00ff4422, 0x002288ff, 0x0066ff44 };
	int color_count = sizeof(colors) / sizeof(int);
	for (int i = 0; i < editor.document.curve_list.size(); i++) {
		if (the_curve_editor.curve_index == i) {
			continue;
		}
		int color = colors[i % color_count];
		color |= 0x44000000; // set alpha
		auto& curve = editor.document.curve_list[i];
		render.render_curve(curve, color);
	}

	if (the_curve_editor.is_selection_valid()) {
		int i = the_curve_editor.curve_index;
		int color = colors[i % color_count];
		auto& curve = editor.document.curve_list[i];
		color |= 0xaa000000; // set alpha
		render.render_curve(curve, color);
	}

	if (tool_instance) {
		const auto props =
			tool_base::rprops{
				curve_to_screen,
				screen_to_curve
		};
		tool_instance->render(gfx, props);
		tool_instance->render(render);
	}

	if (!gfx.AreBoundsValid()) {
		throw "oh crap!";
	}

	onredraw();
}

Application::Application(Gfx320x200& gfx) 
	: gfx(gfx)
	, app_thread(std::thread([this]{ ThreadMethod(); }))
	, onredraw([]{})
	, is_running(true)
	, the_curve_editor{ editor.document, 0 }
	, device(gfx)
	, render(device)
{
	// dont init here, it will cause race condition, do all init in ThreadMethod
}

Application::~Application()
{
	is_running = false;
	app_thread.join();
}

void Application::ShiftView(int amount)
{
	if (amount == 0) return;
	curve_to_screen.translate_x += amount * curve_to_screen.scale_x;
	screen_to_curve = curve_to_screen.inverse();
	signal.notify_all();
}

static void apply_zoom(transformation& t, 
	float factor, int mouse_x, int mouse_y)
{
	t.scale_x *= factor;
	t.scale_y *= factor;
	t.translate_x = (t.translate_x - mouse_x)*factor + mouse_x;
	t.translate_y = (t.translate_y - mouse_y)*factor + mouse_y;
}

void Application::ZoomIn()
{
	apply_zoom(curve_to_screen, 2.0f, mouse_x, mouse_y);
	screen_to_curve = curve_to_screen.inverse();
	signal.notify_all();
}

void Application::ZoomOut()
{
	defer([this] {
		apply_zoom(curve_to_screen, 0.5f, mouse_x, mouse_y);
		screen_to_curve = curve_to_screen.inverse();
	});
}

void Application::Undo()
{
	defer([this]{ this->editor.history.undo(); });
}

void Application::Redo()
{
	defer([this]{ this->editor.history.redo(); });
}

void Application::UpdateLeftButton(bool pressed)
{
	defer([this, pressed] { mouse_l = pressed; });
}

void Application::CancelCurrentEdit()
{
	defer([this] {
		tool_instance.reset();
	});
}

void Application::UpdateMousePos(int x, int y)
{
	if (x != mouse_x || y != mouse_y) {
		mouse_x = x;
		mouse_y = y;
		signal.notify_all();
	}
}

void Application::SetRedrawHandler(std::function<void()> handler)
{
	onredraw = handler;
	onredraw();
	signal.notify_all();
}

void Application::ActivateSplitTool()
{
	defer([this] {
		if (!the_curve_editor.is_selection_valid()) {
			return;
		}
		active_tool_factory([this] {
			return std::make_unique<tool_split>(
				this->editor.document,
				this->the_curve_editor.curve_index);
			}
		);
	});
}

void Application::ActivateEditTool()
{
	defer([this] {
		if (!the_curve_editor.is_selection_valid()) {
			return;
		}
		active_tool_factory([this] {
			return std::make_unique<tool_edit>(
				this->editor.document, 
				this->the_curve_editor.curve_index);
		});
	});
}

void Application::ActivateChangeParamCount() 
{
	defer([this] {
		if (!the_curve_editor.is_selection_valid()) {
			return;
		}
		active_tool_factory([this] {
			return std::make_unique<tool_param_count>(
				this->editor.document,
				this->the_curve_editor.curve_index);
			}
		);
	});
}

std::string Application::ExportCode()
{
	return code_generator::generate_c_like(editor.document);
}

std::string Application::ExportBinary()
{
	std::stringstream ss;
	io_binary::write(ss, editor.document);
	return ss.str();
}

void Application::ImportBinary(const std::string& in_binary)
{
	std::string binary_copy = in_binary;
	defer([this, binary_copy] {
		std::stringstream ss(binary_copy);
		io_binary::read(ss, editor.document);
		this->editor.history.clear();
	});
}

void Application::SelectNextCurve()
{
	defer([this] {
		the_curve_editor.curve_index++;
		if (the_curve_editor.curve_index 
			>= editor.document.curve_list.size()) {
			the_curve_editor.curve_index = 0;
		}
	});
}

void Application::SelectPreviousCurve() 
{
	defer([this] {
		the_curve_editor.curve_index--;
		if (the_curve_editor.curve_index < 0) {
			the_curve_editor.curve_index 
				= editor.document.curve_list.size() - 1;
		}
	});
}

void Application::AddNewCurve()
{
	defer([this] {
		this->editor.history.commit(
			std::make_unique<commands::add_curve>(editor.document));
	});
}
