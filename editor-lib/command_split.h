#pragma once
#include "./command_curve_base.h"

namespace commands
{
	class split : public curve_base
	{
	public:
		double curve_x;

		split(document_model& document, size_t curve_index, double curve_x);

		// Inherited via base
		virtual void execute() override;
		virtual void revert() override;

	private:
		segment original_segment;
	};
}