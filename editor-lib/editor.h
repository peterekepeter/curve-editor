#pragma 
#include <memory>

#include "../curve-lib/document_model.h"

#include "./command_base.h"
#include "./editor_history.h"

class editor
{
public:
	document_model document;
	editor_history history;
};