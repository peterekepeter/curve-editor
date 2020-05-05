#include "stdafx.h"
#include "renderer.h"

renderer::renderer(rendering_device& device)
	: screen(device)
	, curve_to_screen(transformation::identity())
	, screen_to_curve(transformation::identity())
{
}

void renderer::render_separator_lines(const curve& curve, int color)
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

void renderer::render_param_dots(const curve& curve, size_t segment_index, int color)
{
	auto target = curve.get_segment_by_index(segment_index);
	size_t param_count = target.segment.params.size();
	for (size_t i = 0; i < param_count; i++) {
		render_param_dot(curve, segment_index, i, color);
	}
}

void renderer::render_curve(const curve& curve, int color)
{
	int width = screen.get_width();
	int height = screen.get_height();

	// rendering props
	const int max_samples = 5;
	float inv_sub_max = 1.0f / max_samples;

	float t = 0;
	for (int screen_x = 0; screen_x < width; screen_x++) {
		for (int sample = 0; sample < max_samples; sample++) {

			auto screen_x_f = screen_x + sample * inv_sub_max;
			auto curve_x = screen_to_curve.apply_x(screen_x_f);
			auto curve_y = curve.eval(curve_x);
			auto screen_y_f = curve_to_screen.apply_y(curve_y);
			int screen_y = static_cast<int>(screen_y_f);

			if (screen_y >= 0 && screen_y < 200) {
				screen.unsafe_blend_pixel(screen_x, screen_y, color);
			}
		}
	}
}

void renderer::render_param_dot(const curve& curve, size_t segment_index, size_t param_index, int color)
{
	auto target = curve.get_segment_by_index(segment_index);
	size_t param_count = target.segment.params.size();
	size_t last_index = param_count - 1;
	double curve_x;
	if (last_index <= 0) {
		curve_x = 0;
		double weight = 0;
		if (std::isfinite(target.left)) {
			curve_x += target.left;
			weight++;
		}
		if (std::isfinite(target.right)) {
			curve_x += target.right;
			weight++;
		}
		curve_x /= weight;
	}
	else {
		curve_x = target.left + param_index * 
			(target.right - target.left) / last_index;
	}
	
	float curve_y = target.segment.eval(target.left, curve_x, target.right);
	
	int screen_x = static_cast<int>(curve_to_screen.apply_x(curve_x));
	int screen_y = static_cast<int>(curve_to_screen.apply_y(curve_y));

	screen.draw_rect(screen_x - 1, screen_y - 1, 3, 3, color);
}