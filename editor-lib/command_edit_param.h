#pragma once
#include "./command_curve_base.h"

namespace commands
{
	class edit_param : public curve_base
	{
	public:

		edit_param(
			document_model& document,
			size_t curve_index,
			size_t segment_index,
			size_t param_index,
			float new_value);

		// Inherited via base
		virtual void execute() override;
		virtual void revert() override;

	private:
		size_t segment_index;
		size_t param_index;
		float new_value;
		float old_value;

		float& get_param_ref();
	};
}