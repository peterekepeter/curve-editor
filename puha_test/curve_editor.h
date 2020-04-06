#pragma once
#include "../curviness/curve.h"
#include "../puha-render/puha-render.h"
#include "../editor-lib/transformation.h"

class curve_editor
{
public:
	curve* curve;

	struct rprops
	{
		transformation curve_to_screen;
		transformation screen_to_curve;
		int screen_width;
		int screen_height;
	};

	void render(Gfx320x200& gfx, const rprops& props);
};
