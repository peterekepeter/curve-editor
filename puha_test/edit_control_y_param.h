#pragma once
#include "./edit_control.h";
#include "../curviness/segment_with_separators.h"

class edit_control_y_param : public edit_control
{
public:

	edit_control_y_param(
		document_model& document,
		size_t curve_index,
		segment_with_separators<double> target, 
		int param_index);

	// Inherited via edit_control
	virtual void write_edit(float x, float y) override;
	virtual void read_edit(float& x, float& y) override;
	virtual void render(renderer& renderer) override;
	virtual float get_edit_sensitivity() override; 
	virtual tool_base::command_ptr get_command(
		float x_from, float y_from, 
		float x_to, float y_to, 
		float snap_threshold) override;

private:
	segment_with_separators<double> target;
	int param_index;
};