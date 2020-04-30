#include "command_edit_param.h"

commands::edit_param::edit_param(
	document_model& document, 
	size_t curve_index, 
	size_t segment_index, 
	size_t param_index, 
	float new_value)
	: curve_base(document, curve_index)
	, segment_index(segment_index)
	, param_index(param_index)
	, new_value(new_value)
	, old_value(NAN)
{
}

void commands::edit_param::execute()
{
	auto& param_ref = get_param_ref();
	old_value = param_ref;
	param_ref = new_value;
}

void commands::edit_param::revert()
{
	auto& param_ref = get_param_ref();
	param_ref = old_value;
}

float& commands::edit_param::get_param_ref()
{
	auto& curve = get_selected_curve();
	auto& segment = curve.get_segment_ref_by_index(
		segment_index);
	return segment.params[param_index];
}
