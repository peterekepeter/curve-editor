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
	std::thread app_thread;
public:
	Application(Gfx320x200& gfx);
	~Application();

	void OnRedraw(std::function<void()> handler);
};

