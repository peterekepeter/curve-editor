#pragma once
#include "../puha-render/puha-render.h"


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
};