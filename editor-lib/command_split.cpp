#include "command_split.h"

command::split::split(document_model& document, size_t curve_index, double curve_x)
	: curve_base(document, curve_index)
	, curve_x(curve_x)
{
}

void command::split::execute()
{
	auto& curve = get_selected_curve();
	curve.split(curve_x);
}

void command::split::revert()
{
	auto& curve = get_selected_curve();
	auto index = curve.find_separator_index(curve_x);
	curve.remove_split(index);
}
