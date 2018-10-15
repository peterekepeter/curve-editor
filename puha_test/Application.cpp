#include "stdafx.h"
#include "../puha-render/puha-render.h"
#include "Application.h"
#include "../curviness/Curves.h"


float linear_to_srgb(float linear) {
	if (linear < 0.0031308f) {
		return linear * 12.92f;
	}
	else {
		return 1.055f * std::pow(linear, 1.0f / 2.4f) - 0.055f;
	}
}

void Application::ThreadMethod()
{
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

float segmentData[] = { 0.0, 0.0, 1.0, 1.0, 0.5, 0.6, 0.9, -0.2, 1.3, 1.0, 0.0, 0.4, 0.6, 0.6, 0.7, 0.1 };
int segmentDataCount = 4;
int segmentDataAdd = 0;
int nearest = 0;

bool Application::DoWork()
{
	if (segmentDataAdd != 0) {
		segmentDataCount += segmentDataAdd;
		segmentDataAdd = 0;
		if (segmentDataCount > 8)segmentDataCount = 8;
		if (segmentDataCount < 1)segmentDataCount = 1;
	}

	gfx.SetColor(0);
	gfx.RectangleFilled(0, 0, 319, 199);
	gfx.SetColor(0x222222);
	//gfx.HLine(0, mouse_y, 319, mouse_y);
	//gfx.VLine(mouse_x, 0, mouse_x, 199);

	// input data
	int segmentBegin = 50;
	int segmentEnd = 250;

	// rendering props
	const int sub_max = 15;
	const int add_val_r = 17;
	const int add_val_g = 17;
	const int add_val_b = 17;
	int y = 50;
	int y_size = 100;
	// render
	int segmentLength = segmentEnd - segmentBegin;
	gfx.SetColor(0xffffff);
	float t = 0;
	for (int i = 0; i < segmentLength; i++) {
		int xpos = segmentBegin + i;
		for (int sub = 0; sub < sub_max; sub++) {
			float t = i * sub_max + sub;
			t /= segmentLength * sub_max;
			float v = algorithms::basic::generalized_bezier(t, segmentData, segmentDataCount);
			int ypos = y + v * y_size;
			if (ypos > 0 && ypos < 200) {
				auto p = gfx.GetBytePtr(xpos, ypos);
				p[0] = blendf(p[0], add_val_r);
				p[1] = blendf(p[1], add_val_g);
				p[2] = blendf(p[2], add_val_b);
			}
		}
	}
	gfx.SetColor(mouse_l ? 0x00ff00 : 0xff0000);
	// gfx.PutPixel(mouse_x, mouse_y);

	// nearest control point
	int last = segmentDataCount - 1;
	if (!mouse_l) 
	{
		if (segmentDataCount == 1)
		{
			nearest = 0;
		} else 
		if (mouse_x < segmentBegin) {
			nearest = 0;
		}
		else if (mouse_x > segmentEnd) {
			nearest = last;
		}
		else
		{
			nearest = (mouse_x - segmentBegin + (segmentLength / last / 2)) * last / segmentLength;
			if (nearest > last) nearest = last;
		}
	}

	// some logic
	if (mouse_l) {
		segmentData[nearest] += (mouse_y - last_mouse_y) / (float)y_size;
	}

	{
		int nearest_x;
		if (last == 0) {
			nearest_x = segmentBegin + segmentLength / 2;
		}
		else {
			nearest_x = segmentBegin + nearest * segmentLength / last;
		}
		float v = segmentData[nearest];
		int nearest_y = y + v * y_size;
		if (nearest_y > 1 && nearest_y < 198) {
			gfx.RectangleFilled(nearest_x - 1, nearest_y - 1, nearest_x + 1, nearest_y + 1);
		}
	}

	// consume state
	last_mouse_x = mouse_x;
	last_mouse_y = mouse_y;


	onredraw();

	if (segmentDataAdd != 0) {
		return true;
	}
	return false;
}

Application::Application(Gfx320x200& gfx) 
	: gfx(gfx)
	, app_thread(std::thread([this]{ ThreadMethod(); }))
	, onredraw([]{})
	, is_running(true)
{}

Application::~Application()
{
	is_running = false;
	app_thread.join();
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

void Application::OnRedraw(std::function<void()> handler)
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
