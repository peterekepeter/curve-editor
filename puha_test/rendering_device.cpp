#include "stdafx.h"
#include "rendering_device.h"

rendering_device::rendering_device(Gfx320x200& gfx)
	: gfx(gfx)
{
}

int rendering_device::get_width()
{
	return gfx.Width;
}

int rendering_device::get_height()
{
	return gfx.Height;
}

void rendering_device::clear(int color)
{
	gfx.RectFill(0, 0, gfx.Width, gfx.Height, color);
}

void rendering_device::draw_vertical_line(int x, int color)
{
	gfx.CreateViewport().VLine(x, 0, x, gfx.Height, color);
}

void rendering_device::draw_rect(int x, int y, int w, int h, int color)
{
	gfx.CreateViewport().RectFill(x, y, w, h, color);
}
