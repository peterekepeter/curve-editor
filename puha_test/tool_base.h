#pragma once
#include "../editor-lib/command_base.h"


class tool_base
{
public: 
	using command_ptr = std::unique_ptr<commands::base>;

	virtual command_ptr get_command() = 0;

	virtual void mouse_l_release();
	virtual void update_mouse_curve(float x, float y);
	virtual bool is_complete();
	virtual ~tool_base();
};