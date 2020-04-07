#pragma once
#include <memory>
#include "../curviness/curve.h"
#include "../puha-render/puha-render.h"
#include "../editor-lib/transformation.h"
#include "edit_control.h"

class curve_editor
{
public:
	curve* curve;

	struct rprops
	{
		transformation curve_to_screen;
		transformation screen_to_curve;
	};

	void render(Gfx320x200& gfx, const rprops& props);

	struct nearest_result
	{
		float distance;
		std::unique_ptr<edit_control> control;
	};

	nearest_result get_nearest_edit_control(float x, float y);
};
