#include "stdafx.h"
#include "edit_control_y_param.h"
#include "../editor-lib/command_edit_param.h"
#include "./theme.h"

edit_control_y_param::edit_control_y_param(
	document_model& document,
	size_t curve_index,
	segment_with_separators<double> target,
	int param_index)
	: edit_control(document, curve_index)
	, target(target)
	, param_index(param_index)
{
	if (param_index < 0 || param_index > target.segment.params.size()) {
		throw "nope";
	}
}

void edit_control_y_param::render(renderer& renderer)
{
	auto& curve = document.curve_list[curve_index];
	renderer.param_dot(
		curve, 
		target.segment_index,
		param_index, 
		theme::active_color);
}

float edit_control_y_param::get_edit_sensitivity()
{
	return 1.0f;
}

tool_base::command_ptr edit_control_y_param::get_command(float x_from, float y_from, float x_to, float y_to)
{
	float original_y = target.segment.params[param_index];
	auto new_value = original_y + (y_to - y_from) 
		* get_edit_sensitivity();

	return std::make_unique<commands::edit_param>(
		document, 
		curve_index, 
		target.segment_index,
		param_index, 
		new_value);
}

void edit_control_y_param::write_edit(float x, float y)
{
	target.segment.params[param_index] = y;
}

void edit_control_y_param::read_edit(float& x, float& y)
{
	y = target.segment.params[param_index];
}