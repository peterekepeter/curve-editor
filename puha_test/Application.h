#pragma once
#include "../curviness/curviness.h"

class Application
{
	Gfx320x200& gfx;
	std::function<void()> onredraw;
	bool is_running;
	std::mutex thread_mutex;
	std::condition_variable signal;
	void ThreadMethod();
	bool DoWork();

	bool mouse_l = false;
	bool edit_mode = false;

	int mouse_x = 0, mouse_y = 0;
	int last_mouse_x = 0, last_mouse_y = 0;

	int view_x_from = 0, view_x_to = 320;
	float view_y_from = .0f, view_y_to = 1.0f;

	curve the_curve;
	int segmentDataAdd = 0;

	// should be last
	std::thread app_thread;
public:
	Application(Gfx320x200& gfx);
	~Application();

	void ToggleEditMode();

	void ShiftView(int amount);
	void UpdateLeftButton(bool pressed);
	void UpdateMousePos(int x, int y);
	void OnRedraw(std::function<void()> handler);
	void IncreasePoints();
	void DecreasePoints();
};
