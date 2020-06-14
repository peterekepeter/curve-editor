#pragma once
#include "command_base.h"


namespace commands
{
	class add_curve : public base
	{
	public:

		add_curve(document_model& document);

		// Inherited via base
		virtual void execute() override;
		virtual void revert() override;
	};
}