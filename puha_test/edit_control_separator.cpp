#include "stdafx.h"
#include "./edit_control_separator.h"
#include "../editor-lib/command_edit_separator.h"
#include "./theme.h"

edit_control_separator::edit_control_separator(
	document_model& document,
	size_t curve_index, 
	curve& target, 
	size_t separator_index)
	: edit_control(document, curve_index)
	, the_curve(target)
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
	if (separator_index >= the_curve.get_separator_count()) {
		return;
	}
	auto curve_x = the_curve.get_separator_value(separator_index);
	auto screen_width = gfx.Width;
	auto screen_height = gfx.Height;
	auto x = props.curve_to_screen.apply_x(curve_x);
	if (0 <= x && x < screen_width) {
		gfx.Line(x, 0, x , screen_height - 1, theme::active_color);
	}
}

tool_base::command_ptr edit_control_separator::get_command(
	float x_from, float y_from, 
	float x_to, float y_to)
{
	double original_value = the_curve.get_separator_value(separator_index);
	return std::make_unique<commands::edit_separator>(
		document, curve_index, original_value, x_to);
}
