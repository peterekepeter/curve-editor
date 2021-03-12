#pragma once
#include "../curve-lib/document_model.h"

namespace commands
{
	class base
	{
	public:
		bool did_execute = false;
		document_model& document;

		base(document_model& document);

		virtual ~base();

		void exec();
		void undo();

	protected:
		virtual void execute() = 0;
		virtual void revert() = 0;

	};
}