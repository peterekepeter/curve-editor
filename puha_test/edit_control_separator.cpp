#include "stdafx.h"
#include "./edit_control_separator.h"

edit_control_separator::edit_control_separator(curve& target, size_t separator_index)
	: the_curve(target)
	, separator_index(separator_index)
{}

void edit_control_separator::write_edit(float x, float y)
{
	the_curve.set_separator_value(separator_index, x);
}

void edit_control_separator::read_edit(float& x, float& y)
{
	x = the_curve.get_separator_value(separator_index);
}

void edit_control_separator::render(Gfx320x200& gfx, const rprops& props)
{
	edit_control::render(gfx, props);
	auto curve_x = the_curve.get_separator_value(separator_index);
	auto screen_width = gfx.Width;
	auto screen_height = gfx.Height;
	auto x = props.curve_to_screen.apply_x(curve_x);
	if (0 <= x && x < screen_width) {
		gfx.Line(x, 0, x , screen_height - 1);
	}
}
