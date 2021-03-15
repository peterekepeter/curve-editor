#pragma once
#include "../render-lib/puha-render.h"


class rendering_device
{
private:
	Gfx320x200& gfx;

public:

	rendering_device(Gfx320x200& gfx);

	int get_width();
	int get_height();
	void clear(int color);
	void draw_vertical_line(int x, int color);
	void draw_rect(int x, int y, int w, int h, int color);
	void unsafe_blend_pixel(int x, int y, int color);
};