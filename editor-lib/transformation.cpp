#include "transformation.h"

transformation transformation::identity()
{
	return transformation{ 1, 1, 0, 0 };
}

transformation transformation::inverse() const
{
	return transformation
	{ 
		1.0f /scale_x, 
		1.0f / scale_y, 
		-translate_x / scale_x, 
		-translate_y / scale_y 
	};
}

float transformation::apply_x(float in_x) const
{
	return apply_scaling_x(in_x) + translate_x;
}

float transformation::apply_y(float in_y) const
{
	return apply_scaling_y(in_y) + translate_y;
}

point transformation::apply(point in) const
{
	return point{ apply_x(in.x), apply_y(in.y) };
}

float transformation::apply_scaling_x(float in_x) const
{
	return in_x * scale_x;
}

float transformation::apply_scaling_y(float in_y) const
{
	return in_y * scale_y;
}
