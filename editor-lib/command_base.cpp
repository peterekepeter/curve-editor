#include "command_base.h"

commands::base::base(document_model& document)
	: document(document) { }

commands::base::~base() { }

void commands::base::exec()
{
	if (did_execute == true) {
		throw "cannot execute twice";
	}
	execute();
	did_execute = true;
}

void commands::base::undo()
{
	if (did_execute == false) {
		throw "cannot undo if not executed";
	}
	revert();
	did_execute = false;
}
