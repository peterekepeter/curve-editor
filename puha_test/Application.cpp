﻿#include "stdafx.h"
#include "../puha-render/puha-render.h"
#include "Application.h"
#include "../curviness/curviness.h"
#include "../editor-lib/editor_math.h"
#include "../editor-lib/command_split.h"
#include "./tool_split.h"
#include "./tool_edit.h"
#include "./tool_param_count.h"

static void init_curve(curve& c);

void Application::ThreadMethod()
{
	// init code
	editor.document.curve_list.emplace_back();
	auto& the_curve = editor.document.curve_list[0];
	init_curve(the_curve);
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

static void init_curve(curve& c) {
	c.split(-.4f);
	c.split(.7f);
	c.split(1.9f);
	auto& seg0 = c.find_segment(-.5f);
	seg0.params.resize(1);
	seg0.params[0] = -.5f;
	auto& seg1 = c.find_segment(.5f);
	seg1.params.resize(1);
	seg1.params[0] = -.5f;
	auto& seg2 = c.find_segment(.8f);
	seg2.params.resize(8);
	seg2.params[1] = +.5f;
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

	if (preview_command) {
		preview_command->undo();
		preview_command.reset();
	}

	if (tool_active && tool_instance) {
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
			tool_instance.reset();
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

void Application::DoRenderingWork()
{
	const int screen_width = gfx.Width;
	const int screen_height = gfx.Height;

	// Redraw ///////////////////////////////

	gfx.SetColor(0);
	gfx.RectangleFilled(0, 0, 319, 199);
	gfx.SetColor(0x222222);

	the_curve_editor.render(gfx, 
		curve_editor::rprops{
			curve_to_screen,
			screen_to_curve
		}
	);

	if (tool_active && tool_instance) {
		const auto props =
			tool_base::rprops{
				curve_to_screen,
				screen_to_curve
		};
		tool_instance->render(gfx, props);
	}

	//bool is_hover = false, is_active = false;
	//if (tool_edit_active) {
	//	if (mouse_l) {
	//		is_active = true;
	//	}
	//	else {
	//		is_hover = true;
	//	}
	//}

	//if (is_active || is_hover)
	//{
	//	if (target.control)
	//	{
	//		target.control->render(gfx, edit_control::rprops
	//			{
	//				curve_to_screen,
	//				screen_to_curve,
	//				is_hover,
	//				is_active
	//			}
	//		);
	//	}
	//}

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
		tool_active = false;
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

void Application::IncreasePoints()
{
	defer([this] {
		/*the_curve_editor.change_param_count(
			+1, mouse_curve_x);*/
	});
}

void Application::DecreasePoints()
{
	defer([this] {
		/*the_curve_editor.change_param_count(
			-1, mouse_curve_x);*/
	});
}

void Application::SplitCurve()
{
	defer([this] {
		tool_active = true;
		tool_instance = std::make_unique<tool_split>(
			editor.document, 0);
		tool_instance->update_mouse_curve(
			mouse_curve_x, mouse_curve_y);
		tool_instance->update_mouse_screen(
			mouse_x, mouse_y);
	});
}

void Application::ToggleEditMode()
{
	defer([this] {
		tool_active = true;
		tool_instance = std::make_unique<tool_edit>(
			editor.document, 0);
		tool_instance->update_mouse_curve(
			mouse_curve_x, mouse_curve_y);
		tool_instance->update_mouse_screen(
			mouse_x, mouse_y);
	});
}

void Application::ChangeParamCount() 
{
	defer([this] {
		tool_active = true;
		tool_instance = std::make_unique<tool_param_count>(
			editor.document, 0);
		tool_instance->update_mouse_curve(
			mouse_curve_x, mouse_curve_y);
		tool_instance->update_mouse_screen(
			mouse_x, mouse_y);
	});
}