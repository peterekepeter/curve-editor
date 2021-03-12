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

static void unpack_color(int& r, int& g, int& b, int& a, int color) {
	a = int((unsigned(color) & 0xff000000) >> 24);
	r = int((unsigned(color) & 0x00ff0000) >> 16);
	g = int((unsigned(color) & 0x0000ff00) >> 8);
	b = int((unsigned(color) & 0x000000ff));
}

static void pack_color(int& col, int r, int g, int b, int a) {
	col = ((a & 0xff) << 24) 
		| ((r & 0xff) << 16) 
		| ((g & 0xff) << 8) 
		| ((b & 0xff));
}

void rendering_device::unsafe_blend_pixel(int x, int y, int color)
{
	int r, g, b, a;
	unpack_color(r, g, b, a, color);
	int inv_a = 255 - a;
	int dest = gfx.GetPixel(x, y);
	int dr, dg, db, da;
	unpack_color(dr, dg, db, da, dest);
	r = (r * a + dr * inv_a) >> 8;
	g = (g * a + dg * inv_a) >> 8;
	b = (b * a + db * inv_a) >> 8;
	pack_color(color, r, g, b, a);
	gfx.PutPixel(x, y, color);
}
