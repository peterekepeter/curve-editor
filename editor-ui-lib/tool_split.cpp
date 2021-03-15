#include "stdafx.h"
#include "./tool_split.h"
#include "../editor-lib/command_split.h"

tool_split::tool_split(document_model& document, size_t curve_index)
	: document(document)
	, curve_index(curve_index)
{
}

void tool_split::mouse_l_release()
{
	complete = true;
}

void tool_split::update_mouse_curve(float x, float y)
{
	mouse_curve_x = x;
}

bool tool_split::is_complete()
{
	return complete;
}

tool_base::command_ptr tool_split::get_command()
{
	return std::make_unique<commands::split>(
		document, curve_index, mouse_curve_x);
}

void tool_split::render(Gfx320x200& gfx, const rprops& props)
{
	int screen_x = props.curve_to_screen
		.apply_x(mouse_curve_x);
	gfx.SetColor(0xffffff);
	gfx.Line(screen_x, 0, screen_x, gfx.Height-1);
}
