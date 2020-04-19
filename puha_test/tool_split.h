#pragma once
#include "./tool_base.h"

class tool_split : public tool_base
{
	document_model& document;
	size_t curve_index;
	bool complete = false;
	float mouse_curve_x = NAN;

public:

	tool_split(document_model&, size_t curve_index);

	virtual void mouse_l_release() override;
	virtual void update_mouse_curve(float x, float y) override;
	virtual bool is_complete() override;

	// Inherited via tool_base
	virtual command_ptr get_command() override;

};