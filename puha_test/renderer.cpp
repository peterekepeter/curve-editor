#include "stdafx.h"
#include "renderer.h"

renderer::renderer(rendering_device& device)
	: screen(device)
	, curve_to_screen(transformation::identity())
	, screen_to_curve(transformation::identity())
{
}

void renderer::separator_lines(const curve& curve, int color)
{
	auto curve_start_x = screen_to_curve.apply_x(0);
	auto first = curve.find_separator_index(curve_start_x);
	auto count = curve.get_separator_count();
	auto max_screen_x = screen.get_width();
	for (size_t i = first; i < count; i++) {
		auto curve_x = curve.get_separator_value(i);
		auto screen_x = curve_to_screen.apply_x(curve_x);
		screen.draw_vertical_line(screen_x, color);
		if (screen_x > max_screen_x) {
			break;
		}
	}
}
