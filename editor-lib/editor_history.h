#pragma once
#include <memory>
#include <vector>
#include "./command_base.h"
#include "./document_model.h"

class editor_history
{
public:
	bool undo();
	bool redo();
	void commit(std::unique_ptr<commands::base> cmd);
	void clear();

private:
	std::vector<std::unique_ptr<commands::base>> history;
	size_t history_index = 0;
};