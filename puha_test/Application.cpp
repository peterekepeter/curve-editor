#include "stdafx.h"
#include "../puha-render/puha-render.h"
#include "Application.h"
#include "../curviness/curviness.h"

float linear_to_srgb(float linear) {
	if (linear < 0.0031308f) {
		return linear * 12.92f;
	}
	else {
		return 1.055f * std::pow(linear, 1.0f / 2.4f) - 0.055f;
	}
}

static void init_curve(curve& c) {
	c.split(20);
	c.split(100);
	c.split(200);
	auto& seg1 = c.find_segment(21);
	seg1.params.resize(2);
	seg1.params[0] += 1.0f;
	auto& seg2 = c.find_segment(101);
	seg2.params.resize(4);
	seg2.params[1] += .5f;
}

void Application::ThreadMethod()
{
	// init code
	init_curve(the_curve);
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

unsigned char blendf_add(unsigned char dst, unsigned char src) {
	return dst + src;
}

unsigned char blendf(unsigned char dst, unsigned char src) {
	unsigned val = src;
	val *= (255 - dst);
	dst += val >> 8;
	return dst;
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
	// view stuff
	int y = view_y_from;
	int y_size = view_y_to - view_y_from;

	// Update Logic /////////////////////////

	// input segment_mouseover
	auto segment_mouseover = the_curve.get_segment(mouse_x + view_x_from);
	size_t segment_param_count = segment_mouseover.segment.params.size();

	if (segmentDataAdd != 0) {
		segment_param_count += segmentDataAdd;
		segmentDataAdd = 0;
		if (segment_param_count > 8)segment_param_count = 8;
		if (segment_param_count < 1)segment_param_count = 1;
		segment_mouseover.segment.params.resize(segment_param_count);
	}

	int segmentBegin = segment_mouseover.left;
	int segmentEnd = segment_mouseover.right;
	int segmentLength = segmentEnd - segmentBegin;
	
	// nearest control point
	int last_param_index = segment_param_count - 1;
	int nearest;
	int position_x = mouse_x + view_x_from;
	if (segment_param_count == 1)
	{
		nearest = 0;
	}
	else
		if (position_x < segmentBegin) {
			nearest = 0;
		}
		else if (position_x > segmentEnd) {
			nearest = last_param_index;
		}
		else {
			nearest = (position_x - segmentBegin + (segmentLength / last_param_index / 2)) * last_param_index / segmentLength;
			if (nearest > last_param_index) nearest = last_param_index;
		}

	// some logic
	if (edit_mode) {
		if (mouse_l) {
			segment_mouseover.segment.params[nearest] += (mouse_y - last_mouse_y) / (float)y_size;
		}
	}
	else {
		if (mouse_l) {
			auto shift_x = mouse_x - last_mouse_x;
			view_x_from -= shift_x;
			view_x_to -= shift_x;
			auto shift_y = mouse_y - last_mouse_y;
			view_y_from += shift_y;
			view_y_to += shift_y;
		}
	}

	// consume state
	last_mouse_x = mouse_x;
	last_mouse_y = mouse_y;

	app_rendering_state state
	{ 
		segment_mouseover,
		last_param_index, 
		segmentBegin,
		segmentLength,
		nearest
	};

	this->DoRenderingWork(state);

	if (segmentDataAdd != 0) {
		return true;
	}
	return false;
}

void Application::DoRenderingWork(const app_rendering_state& state)
{
	// view stuff
	int y = view_y_from;
	int y_size = view_y_to - view_y_from;

	// Redraw ///////////////////////////////

	gfx.SetColor(0);
	gfx.RectangleFilled(0, 0, 319, 199);
	gfx.SetColor(0x222222);

	//gfx.HLine(0, mouse_y, 319, mouse_y);
	//gfx.VLine(mouse_x, 0, mouse_x, 199);


	const int screenWidth = 320;
	const int screenHeight = 200;

	// draw segment frame
	{
		auto& segment = state.segment_mouseover;
		gfx.SetColor(0x202020);
		int columns[] = { segment.left, segment.right };
		for (int& column : columns) {

			column -= view_x_from;
			if (0 <= column && column < screenWidth)
			{
				gfx.Line(column, 0, column, screenHeight - 1);
			}
		}
		auto& params = segment.segment.params;
		for (float& param : params) {

		}
	}

	// rendering props
	const int sub_max = 15;
	const int add_val_r = 17;
	const int add_val_g = 17;
	const int add_val_b = 17;
	// render
	gfx.SetColor(0xffffff);

	int view_x_length = view_x_to - view_x_from;

	float t = 0;
	for (int i = 0; i < screenWidth; i++) {
		int xpos = view_x_from + i;
		for (int sub = 0; sub < sub_max; sub++) {
			float v = the_curve.eval(xpos + sub / (double)sub_max);
			int ypos = y + int(v * y_size);
			if (ypos > 0 && ypos < 200) {
				auto p = gfx.GetBytePtr(i, ypos);
				p[0] = blendf(p[0], add_val_r);
				p[1] = blendf(p[1], add_val_g);
				p[2] = blendf(p[2], add_val_b);
			}
		}
	}

	if (edit_mode) {
		gfx.SetColor(mouse_l ? 0x00ff00 : 0xff0000);
	}
	else
	{
		gfx.SetColor(0xffffff);
	}
	// gfx.PutPixel(mouse_x, mouse_y);


	// redraw controls
	if (state.segment_mouseover.segment.params.size() > 0)
	{
		int nearest_x;
		if (state.last == 0) {
			nearest_x = state.segmentBegin + state.segmentLength / 2;
		}
		else {
			nearest_x = state.segmentBegin + state.nearest * state.segmentLength / state.last;
		}
		float v = state.segment_mouseover.segment.params[state.nearest];
		// adjust based on view
		nearest_x -= view_x_from;
		int nearest_y = y + int(v * y_size);
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
	view_x_from += amount;
	view_x_to += amount;
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
