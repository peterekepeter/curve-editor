#pragma once
#include <memory>
#include "../curviness/curve.h"
#include "../puha-render/puha-render.h"
#include "../editor-lib/transformation.h"
#include "edit_control.h"

class curve_editor
{
public:
	document_model& document;
	size_t curve_index;

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
		void reset();
	};

	nearest_result get_nearest_edit_control(float x, float y);
	void remove_zero_length_segments();
	void change_param_count(int delta, float x);
};
