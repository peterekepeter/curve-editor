#include "stdafx.h"
#include "Application.h"

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

bool Application::DoWork()
{
	// draw something
	gfx.SetColor(0);
	gfx.RectangleFilled(0, 0, 319, 199);
	gfx.SetColor(0xffffff);
	gfx.Line(0, 0, 64, 64);
	onredraw();
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

void Application::OnRedraw(std::function<void()> handler)
{
	onredraw = handler;
	onredraw();
	signal.notify_all();
}
