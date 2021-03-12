#pragma once
#include "rendering_device.h"
#include "../curve-lib/curve.h"
#include "../editor-lib/transformation.h"

class renderer
{
private:
	rendering_device& screen;

public:
	renderer(rendering_device& device);

	transformation curve_to_screen;
	transformation screen_to_curve;
	
	void render_separator_lines(const curve& curve, int color);
	void render_param_dot(const curve& curve, size_t segment_index, size_t param_index, int color);
	void render_param_dots(const curve& curve, size_t segment_index, int color);
	void render_curve(const curve& curve, int color);
};