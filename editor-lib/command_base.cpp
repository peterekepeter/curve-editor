#include "command_base.h"

command::base::base(document_model& document)
	: document(document) { }

command::base::~base() { }

void command::base::exec()
{
	if (did_execute == true) {
		throw "cannot execute twice";
	}
	execute();
	did_execute = true;
}

void command::base::undo()
{
	if (did_execute == false) {
		throw "cannot undo if not executed";
	}
	revert();
	did_execute = false;
}
