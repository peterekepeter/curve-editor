#pragma once
#include "point.h"

struct transformation
{
	float scale_x, scale_y;
	float translate_x, translate_y;

	static transformation identity();

	transformation inverse() const;

	float apply_x(float) const;

	float apply_y(float) const;

	point apply(point) const;
};

