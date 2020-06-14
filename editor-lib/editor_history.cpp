#include "editor_history.h"

bool editor_history::undo()
{
	if (history_index > 0) {
		history[history_index - 1]->undo();
		history_index--;
		return true;
	}
	else {
		return false;
	}
	return false;
}

bool editor_history::redo()
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

void editor_history::commit(std::unique_ptr<commands::base> cmd)
{
	if (history_index < history.size()) {
		history.resize(history_index);
	}
	cmd->exec();
	history.push_back(std::move(cmd));
	history_index = history.size();
}

void editor_history::clear() {
	history.clear();
	history_index = 0;
}