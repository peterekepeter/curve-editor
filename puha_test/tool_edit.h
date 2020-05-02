#pragma once
#include "./tool_base.h"
#include "./curve_editor.h"

class tool_edit : public tool_base
{
	document_model& document;
	size_t curve_index;
	bool complete = false;
	float x_from = NAN;
	float y_from = NAN;
	float x_to = NAN;
	float y_to = NAN;
	curve_editor::nearest_result target;
	curve_editor editor;
	bool mouse_l_state = false;
	transformation curve_to_screen;

public:

	tool_edit(document_model&, size_t curve_index);

	virtual void mouse_l_press() override;
	virtual void mouse_l_release() override;
	virtual void update_mouse_curve(float, float) override;
	virtual void transform_change(const transformation& curve_to_screen) override;
	virtual bool is_complete() override;


	// Inherited via tool_base
	virtual command_ptr get_command() override;
	virtual void render(Gfx320x200& gfx, const rprops& props) override;
	virtual void render(renderer& render) override;

};
