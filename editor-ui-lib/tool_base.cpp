#include "stdafx.h"
#include "tool_base.h"

void tool_base::mouse_l_press()
{
}

void tool_base::mouse_l_release()
{

}

void tool_base::update_mouse_screen(float x, float y)
{

}

void tool_base::update_mouse_curve(float x, float y)
{

}

bool tool_base::is_complete()
{
	return false;
}

void tool_base::transform_change(const transformation& curve_to_screen)
{
}

tool_base::~tool_base()
{

}

void tool_base::render(Gfx320x200& gfx, const rprops& props)
{

}

void tool_base::render(renderer& render)
{
}
