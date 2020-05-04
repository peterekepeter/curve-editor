#include "stdafx.h"
#include "tool_param_count.h"
#include "../editor-lib/command_change_segment.h"

tool_param_count::tool_param_count(
	document_model& document, 
	size_t curve_index)
	: document(document)
	, curve_index(curve_index)
{

}

void tool_param_count::mouse_l_press()
{
	mouse_pressed = true;
	mouse_press_curve_x = mouse_curve_x;
	mouse_press_y = mouse_y;
	auto& curve = document.curve_list[curve_index];
	segment_index = curve.find_segment_index(mouse_press_curve_x);
}

void tool_param_count::mouse_l_release()
{
	mouse_pressed = false;
	complete = true;
}

void tool_param_count::update_mouse_curve(float x, float y)
{
	mouse_curve_x = x;
	if (!mouse_pressed) {
		segment_index = document.curve_list[curve_index]
			.find_segment_index(x);
	}
}

void tool_param_count::update_mouse_screen(float x, float y)
{
	mouse_y = y;
	if (!mouse_pressed) {
		mouse_press_y = mouse_y;
	}
}

bool tool_param_count::is_complete()
{
	return complete;
}

void tool_param_count::render(renderer& render)
{
	auto& curve = document.curve_list[curve_index];
	render.render_param_dots(curve, segment_index, 0xffffff);
}

using std::vector;

static vector<float> rescale(
	const segment& data,
	int new_param_count)
{
	vector<float> result;
	result.resize(new_param_count);
	for (int i = 0; i < new_param_count; i++) 
	{
		if (new_param_count >= 2) {
			float x = (i) / float(new_param_count - 1);
			result[i] = data.eval(x);
		}
		else 
		{
			float y = data.eval(0.25f) + data.eval(0.75f);
			result[i] = y / 2;
		}
	}
	return result;
}

tool_base::command_ptr tool_param_count::get_command()
{
	int delta_y = -(mouse_y - mouse_press_y) / 8;
	auto& curve = document.curve_list[curve_index];
	auto& target_segment 
		= curve.get_segment_ref_by_index(segment_index);
	auto original_param_count 
		= target_segment.params.size();

	if (target_segment.algorithm != algorithms::generalized_bezier) {
		throw "not supported";
	}

	int new_param_count = delta_y + static_cast<int>(original_param_count);
	if (new_param_count <= min_param_count) {
		new_param_count = min_param_count;
	}
	if (new_param_count >= max_param_count) {
		new_param_count = max_param_count;
	}

	if (new_param_count == original_param_count) {
		// noop
		return std::unique_ptr<commands::base>(); 
	}


	segment change;
	change.algorithm = algorithms::generalized_bezier;
	change.params = rescale(
		target_segment,
		new_param_count);

	return std::make_unique<commands::change_segment>(
		document,
		curve_index,
		segment_index,
		change);
}