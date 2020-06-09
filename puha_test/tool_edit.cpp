#include "stdafx.h"
#include "tool_edit.h"

tool_edit::tool_edit(
	document_model& document, 
	size_t curve_index)
	: document(document)
	, curve_index(curve_index)
	, editor { document, curve_index }
	, complete(false)
{

}

void tool_edit::mouse_l_press()
{
	mouse_l_state = true;
}

void tool_edit::mouse_l_release()
{
	complete = true;
	mouse_l_state = false;
}

void tool_edit::update_mouse_curve(float x, float y)
{
	x_to = x;
	y_to = y;
	if (!mouse_l_state) {
		x_from = x;
		y_from = y;
		target = editor.get_nearest_edit_control(
			x_from, 
			y_from,
			curve_to_screen);
	}
}

void tool_edit::transform_change(const transformation& curve_to_screen)
{
	this->curve_to_screen = curve_to_screen;
}

bool tool_edit::is_complete()
{
	return complete;
}

tool_base::command_ptr tool_edit::get_command()
{
	if (mouse_l_state || complete)
	{
		float snap_threshold = abs(8.0f / curve_to_screen.scale_y);
		return target.control->get_command(
			x_from, y_from, x_to, y_to, snap_threshold);
	}
	else {
		return std::unique_ptr<commands::base>();
	}
}

void tool_edit::render(Gfx320x200& gfx, const rprops& props)
{
	if (target.control) {
		target.control->render(gfx, 
			edit_control::rprops{
				props.curve_to_screen,
				props.screen_to_curve
			});
	}
}

void tool_edit::render(renderer& render)
{
	if (target.control) {
		target.control->render(render);
	}
}
