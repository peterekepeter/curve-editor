#pragma once
#include "./command_curve_base.h"

namespace commands
{
	class change_segment : public curve_base
	{
	public:

		change_segment(
			document_model& document,
			size_t curve_index,
			size_t segment_index,
			segment new_segment);

		// Inherited via base
		virtual void execute() override;
		virtual void revert() override;

	private:
		size_t segment_index;
		segment segment_data, old_segment;
	};
}