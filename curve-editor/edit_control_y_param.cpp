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
	renderer.render_param_dot(
		curve, 
		target.segment_index,
		param_index, 
		theme::active_color);
}

float edit_control_y_param::get_edit_sensitivity()
{
	return 1.0f;
}

tool_base::command_ptr edit_control_y_param::get_command(
	float x_from, float y_from, 
	float x_to, float y_to,
	float snap_threshold)
{
	float original_y = target.segment.params[param_index];
	auto new_value = original_y + (y_to - y_from) 
		* get_edit_sensitivity();

	float snap_distance = 1e9;
	float snap_value = 0;
	// snap logic
	{

		auto& curve = document.curve_list[curve_index];

		if (target.segment.algorithm == algorithms::generalized_bezier) {
			if (param_index == 0 
				&& target.segment_index > 0) 
			{
				auto prev = curve.get_segment_by_index(target.segment_index - 1);
				auto value = prev.eval(target.left);
				auto distance = abs(new_value - value);
				if (distance < snap_distance) {
					snap_value = value;
					snap_distance = distance;
				}
			}
			if (param_index == target.segment.params.size() - 1 
				&& target.segment_index < curve.get_separator_count()) 
			{
				auto next = curve.get_segment_by_index(target.segment_index + 1);
				auto value = next.eval(target.right);
				auto distance = abs(new_value - value);
				if (distance < snap_distance) {
					snap_value = value;
					snap_distance = distance;
				}
			}
		}
	}

	if (snap_distance <= snap_threshold) {
		new_value = snap_value;
	}

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