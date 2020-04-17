#include "command_split.h"

command::split::split(document_model& document, size_t curve_index, double curve_x)
	: curve_base(document, curve_index)
	, curve_x(curve_x)
{
}

void command::split::execute()
{
	auto& curve = get_selected_curve();
	
	// before splitting save segment
	// in case it needs to be reverted
	auto result = curve.get_segment(curve_x);
	original_segment = result.segment;

	curve.split(curve_x);
}

void command::split::revert()
{
	auto& curve = get_selected_curve();
	auto index = curve.find_separator_index(curve_x);
	curve.remove_split(index);

	// restore the original segment
	auto result = curve.get_segment(curve_x);
	result.segment = original_segment;
}
