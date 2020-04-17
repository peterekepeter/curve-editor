#pragma once
#include "./command_curve_base.h"

namespace commands
{
	class edit_separator : public curve_base
	{
	public:

		edit_separator(
			document_model& document, 
			size_t curve_index, 
			double x_from,
			double x_to);

		// Inherited via base
		virtual void execute() override;
		virtual void revert() override;

	private:
		double x_from;
		double x_to;
		bool did_remove = false;
		segment removed_segment;
	};
}