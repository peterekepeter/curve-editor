#pragma once
#include "./tool_base.h"

class tool_param_count : public tool_base
{
	const int min_param_count = 1;
	const int max_param_count = 8;

	document_model& document;
	size_t curve_index;
	bool complete = false;
	bool mouse_pressed = false;
	float mouse_y = NAN;
	float mouse_curve_x = NAN;
	float mouse_press_curve_x = NAN;
	float mouse_press_y = NAN;
	size_t segment_index = 0;

public:

	tool_param_count(document_model&, size_t curve_index);

	virtual void mouse_l_press() override;
	virtual void mouse_l_release() override;
	virtual void update_mouse_curve(float x, float y) override;
	virtual void update_mouse_screen(float x, float y) override;
	virtual bool is_complete() override;
	virtual void render(renderer& render) override;

	// Inherited via tool_base
	virtual command_ptr get_command() override;
};