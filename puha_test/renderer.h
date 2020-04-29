#pragma once
#include "rendering_device.h"
#include "../curviness/curve.h"
#include "../editor-lib/transformation.h"

class renderer
{
private:
	rendering_device& screen;

public:
	renderer(rendering_device& device);

	transformation curve_to_screen;
	transformation screen_to_curve;
	
	void separator_lines(const curve& curve, int color);
};