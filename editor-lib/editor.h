#pragma 
#include <memory>
#include "./command_base.h"
#include "./document_model.h"
#include "./editor_history.h"

class editor
{
public:
	document_model document;
	editor_history history;
};