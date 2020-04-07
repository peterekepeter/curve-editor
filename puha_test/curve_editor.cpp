#include "stdafx.h"
#include "../editor-lib/editor_math.h"
#include "./curve_editor.h"
#include "./edit_control_y_param.h"

static unsigned char blendf_add(unsigned char dst, unsigned char src);
static unsigned char blendf(unsigned char dst, unsigned char src);

void curve_editor::render(Gfx320x200& gfx, const rprops& props)
{
	int screen_width = gfx.Width;
	int screen_height = gfx.Height;

	// rendering props
	const int max_samples = 5;
	const int add_val_r = 2 * 44 / max_samples;
	const int add_val_g = 2 * 188 / max_samples;
	const int add_val_b = 2 * 32 / max_samples;
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

static int get_nearest_param_index(
	const segment_with_separators<double>& target,
	double x_position
);

curve_editor::nearest_result
curve_editor::get_nearest_edit_control(float x, float y)
{
	auto nearest_segment = curve->get_segment(x);
	auto param_index = get_nearest_param_index(nearest_segment, x);
	auto param_distance = curve->eval(x);

	//if (segmentDataAdd != 0) {
	//	segment_param_count += segmentDataAdd;
	//	segmentDataAdd = 0;
	//	if (segment_param_count > 8)segment_param_count = 8;
	//	if (segment_param_count < 1)segment_param_count = 1;
	//	segment_mouseover.segment.params.resize(segment_param_count);
	//}

	//
	return nearest_result
	{
		param_distance,
		std::make_unique<edit_control_y_param>(
			nearest_segment, param_index)
	};
}

static int get_nearest_param_index(
	const segment_with_separators<double>& target,
	double x_position
) {
	auto left = target.left;
	auto right = target.right;
	size_t param_count = target.segment.params.size();
	auto length = right - left;
	
	// nearest control point
	int last_param_index = param_count - 1;
	int nearest_param_index;
	if (param_count == 1)
	{
		nearest_param_index = 0;
	}
	else if (x_position < left) 
	{
		nearest_param_index = 0;
	}
	else if (x_position > right) 
	{
		nearest_param_index = last_param_index;
	}
	else 
	{
		nearest_param_index = static_cast<int>((x_position - left
			+ (length / last_param_index / 2))
			* last_param_index / length);
		nearest_param_index =
			editor_math::clamp(nearest_param_index,
				0, last_param_index);
	}
	
	return nearest_param_index;
}