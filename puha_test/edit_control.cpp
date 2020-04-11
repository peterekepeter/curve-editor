#include "stdafx.h"
#include "edit_control.h"

float edit_control::get_edit_sensitivity()
{
	return 1.0f;
}

edit_control::edit_control()
	: original_x(NAN)
	, original_y(NAN)
	, edit_x(0)
	, edit_y(0)
	, dirty(false) 
{}

edit_control::~edit_control()
{

}

void edit_control::add_edit(float dx, float dy)
{
	if (!dirty) {
		dirty = true;
		read_edit(original_x, original_y);
	}
	edit_x += dx;
	edit_y += dy;

	write_edit(
		original_x + edit_x, 
		original_y + edit_y);
}

void edit_control::resolve_edit()
{
	if (dirty) {
		write_edit(
			original_x + edit_x, 
			original_y + edit_y);
	}
}

void edit_control::reject_edit()
{
	if (dirty) {
		write_edit(original_x, original_y);
	}
}

void edit_control::render(Gfx320x200& gfx, const rprops& props)
{
	if (props.is_hover) {
		gfx.SetColor(0xffffff);
	}
	if (props.is_active) {
		gfx.SetColor(0xff0000);
	}
}
