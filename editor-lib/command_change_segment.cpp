#include "command_change_segment.h"

commands::change_segment::change_segment(
	document_model& document,
	size_t curve_index,
	size_t segment_index,
	segment new_segment)
	: curve_base(document, curve_index)
	, segment_index(segment_index)
	, segment_data(new_segment)
{

}

void commands::change_segment::execute()
{
	auto& curve = get_selected_curve();
	auto& target = curve.get_segment_ref_by_index(segment_index);
	std::swap(target, segment_data);
}

void commands::change_segment::revert()
{
	auto& curve = get_selected_curve();
	auto& target = curve.get_segment_ref_by_index(segment_index);
	std::swap(target, segment_data);
}
