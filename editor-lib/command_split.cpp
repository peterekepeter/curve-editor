#include "command_split.h"

commands::split::split(document_model& document, size_t curve_index, double curve_x)
	: curve_base(document, curve_index)
	, curve_x(curve_x)
{
}

void commands::split::execute()
{
	auto& curve = get_selected_curve();
	
	// before splitting save segment
	// in case it needs to be reverted
	auto result = curve.get_segment(curve_x);
	original_segment = result.segment;

	curve.split(curve_x);
}

void commands::split::revert()
{
	auto& curve = get_selected_curve();
	auto index = curve.find_separator_index(curve_x);
	curve.remove_split(index);

	// restore the original segment
	auto result = curve.get_segment(curve_x);
	result.segment = original_segment;
}
