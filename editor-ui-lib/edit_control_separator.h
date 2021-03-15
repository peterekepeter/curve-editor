#pragma once
#include "../curve-lib/curve.h";
#include "./edit_control.h";

class edit_control_separator : public edit_control
{
public:

	edit_control_separator(
		document_model& document, 
		size_t curve_index, 
		curve& curve, 
		size_t separator_index);

	// Inherited via edit_control
	virtual void write_edit(float x, float y) override;
	virtual void read_edit(float& x, float& y) override;
	virtual void render(Gfx320x200& gfx, const rprops& props) override;

	virtual tool_base::command_ptr get_command(
		float x_from, float y_from,
		float x_to, float y_to,
		float snap_threshold) override;

private:

	curve& the_curve;
	size_t separator_index;

};