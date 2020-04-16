#pragma once
#include "command_base.h"

namespace command
{
	class curve_base : public base
	{
	public:
		size_t curve_index;

		curve_base(document_model& doc, size_t index);

	protected:
		curve& get_selected_curve();
	};
}