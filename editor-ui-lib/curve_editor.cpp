#include "stdafx.h"
#include "../editor-lib/editor_math.h"
#include "./curve_editor.h"
#include "./edit_control_y_param.h"
#include "./edit_control_separator.h"

static int get_nearest_param_index(
	const segment_with_separators<double>& target,
	double x_position
);

curve_editor::nearest_result
curve_editor::get_nearest_edit_control(
	float x, 
	float y, 
	const transformation& curve_to_screen)
{
	if (!is_selection_valid()) {
		return nearest_result{}; // empty
	}
	auto& curve = document.curve_list[curve_index];
	auto nearest_segment = curve.find_segment(x);
	auto param_index = get_nearest_param_index(nearest_segment, x);
	
	float bias_towards_param_pixels = 8;

	auto param_distance = abs(
		curve_to_screen.apply_scaling_y(
			curve.eval(x) - y)) 
		- bias_towards_param_pixels;
	
	auto left_distance = abs(
		curve_to_screen.apply_scaling_x(
			float(x - nearest_segment.left)));

	auto right_distance = abs(
		curve_to_screen.apply_scaling_x(
			float(nearest_segment.right - x)));

	auto best_distance = editor_math::min(
		param_distance, left_distance, right_distance);

	if (best_distance == param_distance) 
	{
		return nearest_result
		{
			param_distance + bias_towards_param_pixels,
			std::make_unique<edit_control_y_param>(
				document,
				curve_index,
				nearest_segment, 
				param_index)
		};
	}

	if (best_distance == left_distance)
	{
		size_t separator_index = curve.find_separator_index(
			nearest_segment.left);

		return nearest_result
		{
			left_distance,
			std::make_unique<edit_control_separator>(
				document,
				curve_index,
				curve, 
				separator_index)
		};
	}

	if (best_distance == right_distance)
	{
		size_t separator_index = curve.find_separator_index(
			nearest_segment.right);

		return nearest_result
		{
			right_distance,
			std::make_unique<edit_control_separator>(
				document, 
				curve_index,
				curve, 
				separator_index)
		};
	}

	return nearest_result{}; // empty
}

void curve_editor::remove_zero_length_segments()
{
	auto& curve = document.curve_list[curve_index];
	curve.remove_zero_length_segments();
}

void curve_editor::change_param_count(int segmentDataAdd, float x)
{
	auto& curve = document.curve_list[curve_index];
	auto target = curve.find_segment(x);
	auto segment_param_count = target.segment.params.size();
	if (segmentDataAdd != 0) {
		segment_param_count += segmentDataAdd;
		segmentDataAdd = 0;
		if (segment_param_count > 8)segment_param_count = 8;
		if (segment_param_count < 1)segment_param_count = 1;
		target.segment.params.resize(segment_param_count);
	}
}

static int get_nearest_param_index(
	const segment_with_separators<double>& target,
	double x_position
) {
	auto left = target.left;
	auto right = target.right;
	size_t param_count = target.segment.params.size();
	auto length = right - left;
	
	// nearest control point
	int last_param_index = param_count - 1;
	int nearest_param_index;
	if (param_count == 1)
	{
		nearest_param_index = 0;
	}
	else if (x_position < left) 
	{
		nearest_param_index = 0;
	}
	else if (x_position > right) 
	{
		nearest_param_index = last_param_index;
	}
	else 
	{
		nearest_param_index = static_cast<int>((x_position - left
			+ (length / last_param_index / 2))
			* last_param_index / length);
		nearest_param_index =
			editor_math::clamp(nearest_param_index,
				0, last_param_index);
	}
	
	return nearest_param_index;
}

void curve_editor::nearest_result::reset()
{
	distance = NAN;
	control.reset(nullptr);
}
