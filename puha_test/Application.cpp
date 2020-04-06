#include "stdafx.h"
#include "../puha-render/puha-render.h"
#include "Application.h"
#include "../curviness/curviness.h"
#include "../editor-lib/editor_math.h"

float linear_to_srgb(float linear) {
	if (linear < 0.0031308f) {
		return linear * 12.92f;
	}
	else {
		return 1.055f * std::pow(linear, 1.0f / 2.4f) - 0.055f;
	}
}


static void init_curve(curve& c);

void Application::ThreadMethod()
{
	// init code
	init_curve(the_curve);
	curve_to_screen = transformation{ 50, -100, 160, 100 };
	screen_to_curve = curve_to_screen.inverse();

	the_curve_editor.curve = &the_curve;
	
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
	seg1.params.resize(2);
	seg1.params[0] = -.5f;
	auto& seg2 = c.find_segment(.8f);
	seg2.params.resize(5);
	seg2.params[1] = +.5f;
}


struct rect
{
	int left, right, top, bottom;
	bool is_inside(int x, int y) { return 
		left < x && x < right && 
		top < y && y < bottom; 
	}
	int get_width() { return right - left; }
	int get_height() { return bottom - top; }
};

bool Application::DoWork()
{
	// Update Logic /////////////////////////

	// input segment_mouseover
	float mouse_curve_x = screen_to_curve.apply_x(mouse_x);
	auto segment_mouseover = the_curve.get_segment(mouse_curve_x);
	size_t segment_param_count = segment_mouseover.segment.params.size();

	if (segmentDataAdd != 0) {
		segment_param_count += segmentDataAdd;
		segmentDataAdd = 0;
		if (segment_param_count > 8)segment_param_count = 8;
		if (segment_param_count < 1)segment_param_count = 1;
		segment_mouseover.segment.params.resize(segment_param_count);
	}

	auto segment_start_x = segment_mouseover.left;
	auto segment_end_x = segment_mouseover.right;
	auto segmentLength = segment_end_x - segment_start_x;
	
	// nearest control point
	int last_param_index = segment_param_count - 1;
	int nearest_param_index;
	if (segment_param_count == 1)
	{
		nearest_param_index = 0;
	}
	else
		if (mouse_curve_x < segment_start_x) {
			nearest_param_index = 0;
		}
		else if (mouse_curve_x > segment_end_x) {
			nearest_param_index = last_param_index;
		}
		else {
			nearest_param_index = static_cast<int>((mouse_curve_x - segment_start_x 
				+ (segmentLength / last_param_index / 2))
				* last_param_index / segmentLength);
			nearest_param_index = 
				editor_math::clamp(nearest_param_index, 
					0, last_param_index);
		}

	// some logic
	auto delta_x = mouse_x - last_mouse_x;
	auto delta_y = mouse_y - last_mouse_y;
	if (edit_mode) {
		if (mouse_l) {
			segment_mouseover.segment.params[nearest_param_index] 
				+= screen_to_curve.scale_y * delta_y;
		}
	}
	else {
		if (mouse_l) {
			curve_to_screen.translate_x += 
				delta_x;
			curve_to_screen.translate_y +=
				delta_y;
			screen_to_curve = curve_to_screen.inverse();
		}
	}

	// consume state
	last_mouse_x = mouse_x;
	last_mouse_y = mouse_y;

	app_rendering_state state
	{ 
		segment_mouseover,
		last_param_index, 
		segment_start_x,
		segmentLength,
		nearest_param_index
	};

	this->DoRenderingWork(state);

	if (segmentDataAdd != 0) {
		return true;
	}
	return false;
}

void Application::DoRenderingWork(const app_rendering_state& state)
{
	const int screen_width = 320;
	const int screen_height = 200;

	// Redraw ///////////////////////////////

	gfx.SetColor(0);
	gfx.RectangleFilled(0, 0, 319, 199);
	gfx.SetColor(0x222222);

	//gfx.HLine(0, mouse_y, 319, mouse_y);
	//gfx.VLine(mouse_x, 0, mouse_x, 199);

	// draw segment frame
	{
		auto& segment = state.segment_mouseover;
		gfx.SetColor(0x202020);
		double columns[] = { segment.left, segment.right };
		for (auto& column : columns) {
			column = curve_to_screen.apply_x(column);
			if (0 <= column && column < screen_width)
			{
				gfx.Line(column, 0, column, screen_height - 1);
			}
		}
	}

	the_curve_editor.render(gfx, 
		curve_editor::rprops{
			curve_to_screen,
			screen_to_curve,
			screen_width,
			screen_height,
		}
	);

	if (edit_mode) {
		gfx.SetColor(mouse_l ? 0x00ff00 : 0xff0000);
	}
	else
	{
		gfx.SetColor(0xffffff);
	}
	// gfx.PutPixel(mouse_x, mouse_y);


	// redraw control points
	if (state.segment_mouseover.segment.params.size() > 0)
	{
		double nearest_x_f;
		if (state.last == 0) {
			nearest_x_f = state.segmentBegin + state.segmentLength / 2;
		}
		else {
			nearest_x_f = state.segmentBegin + state.nearest * state.segmentLength / state.last;
		}
		float v = state.segment_mouseover.segment.params[state.nearest];
		// adjust based on view
		int nearest_x = static_cast<int>(curve_to_screen.apply_x(nearest_x_f));
		int nearest_y = static_cast<int>(curve_to_screen.apply_y(v));
		if (nearest_x > 1 && nearest_x < 318 && nearest_y > 1 && nearest_y < 198) {
			gfx.RectangleFilled(nearest_x - 1, nearest_y - 1, nearest_x + 1, nearest_y + 1);
		}
	}

	if (edit_mode) {
		gfx.SetColor(0xff0000);
		gfx.Rectangle(0, 0, 319, 199);
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
{
	// dont init here, it will cause race condition, do all init in ThreadMethod
}

Application::~Application()
{
	is_running = false;
	app_thread.join();
}

void Application::ToggleEditMode()
{
	edit_mode = !edit_mode;
	signal.notify_all();
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
	apply_zoom(curve_to_screen, 0.5f, mouse_x, mouse_y);
	screen_to_curve = curve_to_screen.inverse();
	signal.notify_all();
}

void Application::UpdateLeftButton(bool pressed)
{
	if (pressed != mouse_l) {
		mouse_l = pressed;
		signal.notify_all();
	}
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
	segmentDataAdd++;
	signal.notify_all();
}

void Application::DecreasePoints()
{
	segmentDataAdd--;
	signal.notify_all();
}
