#pragma once
#include "./document_model.h"

namespace command
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