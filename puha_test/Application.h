#pragma once
#include <queue>
#include <functional>
#include <mutex>
#include <sstream>
#include "../curviness/curviness.h"
#include "./curve_editor.h"
#include "../editor-lib/transformation.h"
#include "../editor-lib/editor.h"
#include "../puha-render/puha-render.h"
#include "./tool_base.h"
#include "./rendering_device.h"
#include "./renderer.h"
#include "../editor-lib/io_binary.h"

class Application
{
	Gfx320x200& gfx;
	rendering_device device;
	renderer render;

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
	int mouse_press_x = 0, mouse_press_y = 0;
	float mouse_curve_x, mouse_curve_y;

	transformation curve_to_screen;
	transformation screen_to_curve;

	editor editor;

	std::queue<std::function<void()>> work_items;
	std::mutex work_items_mutex;

	void defer(std::function<void()>);
	void execute_work_items();
	void select_nearest_curve(int x, int y);

	// should be last
	std::thread app_thread;
public:
	Application(Gfx320x200& gfx);
	~Application();

	void ActivateEditTool();
	void ActivateSplitTool();
	void ActivateChangeParamCount();

	void ShiftView(int amount);
	void UpdateLeftButton(bool pressed);
	void CancelCurrentEdit();
	void UpdateMousePos(int x, int y);
	void SetRedrawHandler(std::function<void()> handler);
	void ZoomIn();
	void ZoomOut();
	void Undo();
	void Redo();
	std::string ExportCode();
	std::string ExportBinary();
	void ImportBinary(const std::string&);
	void SelectNextCurve();
	void SelectPreviousCurve();
	void AddNewCurve();
};