#pragma once
#include <queue>
#include <functional>
#include <mutex>
#include "../curviness/curviness.h"
#include "./curve_editor.h"
#include "../editor-lib/transformation.h"
#include "../editor-lib/editor.h"
#include "./tool_base.h"

class Application
{
	Gfx320x200& gfx;
	std::function<void()> onredraw;
	bool is_running;
	std::mutex thread_mutex;
	std::condition_variable signal;
	void ThreadMethod();
	bool DoWork();
	void DoRenderingWork();

	bool mouse_l = false;
	bool mouse_l_prev = false;
	bool tool_active = false;
	std::unique_ptr<tool_base> tool_instance;
	std::unique_ptr<commands::base> preview_command;
	curve_editor the_curve_editor;

	int mouse_x = 0, mouse_y = 0;
	int last_mouse_x = 0, last_mouse_y = 0;
	float mouse_curve_x, mouse_curve_y;

	transformation curve_to_screen;
	transformation screen_to_curve;

	editor editor;

	std::queue<std::function<void()>> work_items;
	std::mutex work_items_mutex;

	void defer(std::function<void()>);
	void execute_work_items();

	// should be last
	std::thread app_thread;
public:
	Application(Gfx320x200& gfx);
	~Application();

	void ToggleEditMode();

	void ShiftView(int amount);
	void UpdateLeftButton(bool pressed);
	void CancelCurrentEdit();
	void UpdateMousePos(int x, int y);
	void SetRedrawHandler(std::function<void()> handler);
	void IncreasePoints();
	void DecreasePoints();
	void SplitCurve();
	void ZoomIn();
	void ZoomOut();
	void Undo();
	void Redo();
	void ChangeParamCount();
};