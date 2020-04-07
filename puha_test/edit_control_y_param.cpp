#include "stdafx.h"
#include "edit_control_y_param.h"

edit_control_y_param::edit_control_y_param(segment_with_separators<double> target, int param_index)
	: target(target)
	, param_index(param_index)
{
	if (param_index < 0 || param_index > target.segment.params.size()) {
		throw "nope";
	}
}

void edit_control_y_param::render(Gfx320x200& gfx, const rprops& props)
{
	edit_control::render(gfx, props); // sets color

	size_t param_count = target.segment.params.size();
	size_t last_index = param_count - 1;
	double curve_x;

	if (last_index <= 0) {
		curve_x = (target.left + target.right) / 2;
	}
	else {
		curve_x = target.left + param_index * (target.right - target.left) 
			/ last_index;
	}

	float curve_y = target.segment.params[param_index];

	int screen_x = static_cast<int>(props.curve_to_screen.apply_x(curve_x));
	int screen_y = static_cast<int>(props.curve_to_screen.apply_y(curve_y));
	if (screen_x > 1 && screen_x < gfx.Width - 2 
		&& screen_y > 1 && screen_y < gfx.Height - 2) {
		gfx.RectangleFilled(screen_x - 1, screen_y - 1, screen_x + 1, screen_y + 1);
	}
}

void edit_control_y_param::write_edit(float x, float y)
{
	target.segment.params[param_index] = y;
}

void edit_control_y_param::read_edit(float& x, float& y)
{
	y = target.segment.params[param_index];
}