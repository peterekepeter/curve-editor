#pragma once
#include "point.h"

struct transformation
{
	float scale_x, scale_y;
	float translate_x, translate_y;

	static transformation identity();

	transformation inverse();

	float apply_x(float);

	float apply_y(float);

	point apply(point);
};

