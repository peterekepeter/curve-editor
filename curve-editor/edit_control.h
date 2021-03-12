#pragma once
#include "../render-lib/puha-render.h"
#include "../editor-lib/transformation.h"
#include "./tool_base.h"
#include "./renderer.h"

class edit_control
{
public:

	document_model& document;
	size_t curve_index;

	// writes the edit into the edited structure
	virtual void write_edit(float x, float y) = 0;
	// reads the edited values from the edited structure
	virtual void read_edit(float& x, float& y) = 0;

	virtual float get_edit_sensitivity();

	edit_control(document_model& document, size_t curve_index);
	virtual ~edit_control();

	void add_edit(float dx, float dy);
	void apply_edit();
	void revert_edit();

	virtual tool_base::command_ptr get_command(
		float x_from, 
		float y_from,
		float x_to, 
		float y_to,
		float snap_threshold) = 0;

	struct rprops
	{
		transformation curve_to_screen;
		transformation screen_to_curve;
		bool is_hover;
		bool is_active;
	};

	virtual void render(Gfx320x200& gfx, const rprops& props);
	virtual void render(renderer& renderer);

private:
	float original_x;
	float original_y;
	float edit_x;
	float edit_y;
	bool dirty;
};
