#pragma once
#include "../curviness/curviness.h"
#include "./curve_editor.h"
#include "../editor-lib/transformation.h"

struct app_rendering_state
{
	segment_with_separators<double> segment_mouseover;
	int last;
	double segmentBegin, segmentLength;
	int nearest;
};

class Application
{
	Gfx320x200& gfx;
	std::function<void()> onredraw;
	bool is_running;
	std::mutex thread_mutex;
	std::condition_variable signal;
	void ThreadMethod();
	bool DoWork();
	void DoRenderingWork(const app_rendering_state&);

	bool mouse_l = false;
	bool edit_mode = false;

	int mouse_x = 0, mouse_y = 0;
	int last_mouse_x = 0, last_mouse_y = 0;

	transformation curve_to_screen;
	transformation screen_to_curve;

	curve the_curve;
	curve_editor the_curve_editor;
	
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
	void SetRedrawHandler(std::function<void()> handler);
	void IncreasePoints();
	void DecreasePoints();
	void ZoomIn();
	void ZoomOut();
};