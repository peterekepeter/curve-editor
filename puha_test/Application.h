#pragma once
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
	int mouse_x = 0, mouse_y = 0;
	int last_mouse_x = 0, last_mouse_y = 0;

	// should be last
	std::thread app_thread;
public:
	Application(Gfx320x200& gfx);
	~Application();

	void UpdateLeftButton(bool pressed);
	void UpdateMousePos(int x, int y);
	void OnRedraw(std::function<void()> handler);
	void IncreasePoints();
	void DecreasePoints();
};

