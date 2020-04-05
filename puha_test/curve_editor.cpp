#include "stdafx.h"
#include "curve_editor.h"

static unsigned char blendf_add(unsigned char dst, unsigned char src);
static unsigned char blendf(unsigned char dst, unsigned char src);

void curve_editor::render(Gfx320x200& gfx, const rprops& props)
{
	int view_x_from = props.view_x_from;
	int view_y_from = props.view_y_from;
	int screen_width = props.screen_width;
	int screen_height = props.screen_height;

	// rendering props
	const int sub_max = 15;
	const int add_val_r = 42;
	const int add_val_g = 17;
	const int add_val_b = 17;
	// render
	gfx.SetColor(0xffffff);

	int view_x_size = props.view_x_to - view_x_from;
	int view_y_size = props.view_y_to - view_y_from;

	float t = 0;
	for (int i = 0; i < screen_width; i++) {
		int xpos = view_x_from + i;
		for (int sub = 0; sub < sub_max; sub++) {
			float v = curve->eval(xpos + sub / (double)sub_max);
			int ypos = view_y_from + int(v * view_y_size);
			if (ypos > 0 && ypos < 200) {
				auto p = gfx.GetBytePtr(i, ypos);
				p[0] = blendf(p[0], add_val_r);
				p[1] = blendf(p[1], add_val_g);
				p[2] = blendf(p[2], add_val_b);
			}
		}
	}
}

static unsigned char blendf_add(unsigned char dst, unsigned char src) {
	return dst + src;
}

static unsigned char blendf(unsigned char dst, unsigned char src) {
	unsigned val = src;
	val *= (255 - dst);
	dst += val >> 8;
	return dst;
}