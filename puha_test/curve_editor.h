#pragma once
#include "../curviness/curve.h"
#include "../puha-render/puha-render.h"

class curve_editor
{
public:
	curve* curve;

	struct rprops
	{
		int view_x_from;
		int view_x_to;
		int view_y_from;
		int view_y_to;
		int screen_width;
		int screen_height;
	};

	void render(Gfx320x200& gfx, const rprops& props);
};
