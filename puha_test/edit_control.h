#pragma once
#include "../puha-render/puha-render.h"
#include "../editor-lib/transformation.h"

class edit_control
{
public:

	// writes the edit into the edited structure
	virtual void write_edit(float x, float y) = 0;
	// reads the edited values from the edited structure
	virtual void read_edit(float& x, float& y) = 0;

	edit_control();
	virtual ~edit_control();

	void add_edit(float dx, float dy);
	void resolve_edit();
	void reject_edit();

	struct rprops
	{
		transformation curve_to_screen;
		transformation screen_to_curve;
		bool is_hover;
		bool is_active;
	};

	virtual void render(Gfx320x200& gfx, const rprops& props);

private:
	float original_x;
	float original_y;
	float edit_x;
	float edit_y;
	bool dirty;
};
