#include "command_curve_base.h"

commands::curve_base::curve_base(document_model& doc, size_t index)
	: base(doc)
	, curve_index(index)
{ }

curve& commands::curve_base::get_selected_curve()
{
	return document.curve_list[curve_index];
}
