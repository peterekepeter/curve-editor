#pragma once
#include "./edit_control.h";
#include "../curviness/curve.h";

class edit_control_separator : public edit_control
{
public:

	edit_control_separator(curve& curve, size_t separator_index);

	// Inherited via edit_control
	virtual void write_edit(float x, float y) override;
	virtual void read_edit(float& x, float& y) override;
	virtual void render(Gfx320x200& gfx, const rprops& props) override;

private:

	curve& the_curve;
	size_t separator_index;
};