#include "stdafx.h"
#include "curve_editor.h"

static unsigned char blendf_add(unsigned char dst, unsigned char src);
static unsigned char blendf(unsigned char dst, unsigned char src);

void curve_editor::render(Gfx320x200& gfx, const rprops& props)
{
	int screen_width = props.screen_width;
	int screen_height = props.screen_height;

	// rendering props
	const int max_samples = 15;
	const int add_val_r = 42;
	const int add_val_g = 17;
	const int add_val_b = 17;
	// render
	gfx.SetColor(0xffffff);

	float inv_sub_max = 1.0f / max_samples;

	float t = 0;
	for (int screen_x = 0; screen_x < screen_width; screen_x++) {
		for (int sample = 0; sample < max_samples; sample++) {

			auto screen_x_f = screen_x + sample * inv_sub_max;
			auto curve_x = props.screen_to_curve.apply_x(screen_x_f);
			auto curve_y = curve->eval(curve_x);
			auto screen_y_f = props.curve_to_screen.apply_y(curve_y);
			int screen_y = static_cast<int>(screen_y_f);

			if (screen_y >= 0 && screen_y < 200) {
				auto p = gfx.GetBytePtr(screen_x, screen_y);
				p[0] = blendf(p[0], add_val_b);
				p[1] = blendf(p[1], add_val_g);
				p[2] = blendf(p[2], add_val_r);
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