#pragma once
#include "./edit_control.h";
#include "../curviness/segment_with_separators.h"

class edit_control_y_param : public edit_control
{
public:

	edit_control_y_param(segment_with_separators<double> target, int param_index);

	// Inherited via edit_control
	virtual void write_edit(float x, float y) override;
	virtual void read_edit(float& x, float& y) override;
	virtual void render(Gfx320x200& gfx, const rprops& props) override;

private:
	segment_with_separators<double> target;
	int param_index;
};