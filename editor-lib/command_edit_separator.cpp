#include "command_edit_separator.h"

command::edit_separator::edit_separator(document_model& document, size_t curve_index, double x_from, double x_to)
	: curve_base(document, curve_index)
	, x_from(x_from)
	, x_to(x_to)
{
}

void command::edit_separator::execute()
{
	auto& curve = get_selected_curve();
	auto index = curve.find_separator_index(x_from);
	size_t separator_count = curve.get_separator_count();

	
	bool remove = false;
	size_t remove_at_index = 0;
	double value = x_to;

	if (x_to > x_from) {
		time_t max_val = +INFINITY;
		if (index + 1 < separator_count) {
			max_val = curve.get_separator_value(index + 1);
		}
		if (value >= max_val) {
			value = max_val;
			remove = true;
			remove_at_index = index;
		}
	} else {
		time_t min_val = -INFINITY;
		if (index > 0) {
			min_val = curve.get_separator_value(index - 1);
		}
		// clamp
		if (value <= min_val) {
			value = min_val;
			remove = true;
			remove_at_index = index - 1;
		}
	}

	curve.set_separator_value(index, value);
	if (remove) {
		// todo cleanup ops
		removed_segment = curve.get_segment_by_index(
			remove_at_index + 1);
		curve.remove_split(remove_at_index);
		did_remove = true;
	}
}

void command::edit_separator::revert()
{
	auto& curve = get_selected_curve();
	if (!did_remove) {
		auto index = curve.find_separator_index(x_to);
		curve.set_separator_value(index, x_from);
	}
	else {
		curve.split(x_from);
		auto result = curve.get_segment((x_from + x_to) / 2);
		result.segment = removed_segment;
	}
}
