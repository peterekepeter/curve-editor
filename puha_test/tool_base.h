#pragma once
#include "../editor-lib/command_base.h"
#include "../editor-lib/transformation.h"
#include "../puha-render/puha-render.h"
#include "./renderer.h"


class tool_base
{
public: 
	using command_ptr = std::unique_ptr<commands::base>;

	virtual command_ptr get_command() = 0;

	virtual void mouse_l_press();
	virtual void mouse_l_release();
	virtual void update_mouse_screen(float x, float y);
	virtual void update_mouse_curve(float x, float y);
	virtual bool is_complete();
	virtual void transform_change(const transformation& curve_to_screen);
	virtual ~tool_base();

	struct rprops
	{
		transformation curve_to_screen;
		transformation screen_to_curve;
	};

	virtual void render(Gfx320x200& gfx, const rprops& props);
	virtual void render(renderer& render);
};