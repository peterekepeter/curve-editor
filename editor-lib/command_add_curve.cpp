#include "command_add_curve.h"

commands::add_curve::add_curve(document_model& document)
	: base(document) 
{
}

void commands::add_curve::execute()
{
	document.curve_list.emplace_back();
}

void commands::add_curve::revert()
{
	document.curve_list.pop_back();
}
