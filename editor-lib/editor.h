#pragma 
#include <memory>
#include "./command_base.h"
#include "./document_model.h"

class editor
{
public:
	document_model document;

	bool undo();
	bool redo();
	void commit(std::unique_ptr<command::base> cmd);

private:
	std::vector<std::unique_ptr<command::base>> history;
	size_t history_index = 0;
};