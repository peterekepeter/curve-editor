#include "document_editor.h"

bool document_editor::undo()
{
	if (history_index > 0) {
		history[history_index - 1]->undo();
		history_index--;
		return true;
	} else {
		return false;
	}
	return false;
}

bool document_editor::redo()
{
	if (history_index < history.size())
	{
		history[history_index]->exec();
		history_index++;
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

void document_editor::commit(std::unique_ptr<command::base> cmd)
{
	if (history_index < history.size()) {
		history.resize(history_index);
	}
	cmd->exec();
	history.push_back(std::move(cmd));
	history_index = history.size();
}
