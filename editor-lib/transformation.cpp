#include "transformation.h"

transformation transformation::identity()
{
	return transformation{ 1, 1, 0, 0 };
}

transformation transformation::inverse()
{
	return transformation
	{ 
		1.0f /scale_x, 
		1.0f / scale_y, 
		-translate_x / scale_x, 
		-translate_y / scale_y 
	};
}

float transformation::apply_x(float in)
{
	return in * scale_x + translate_x;
}

float transformation::apply_y(float in)
{
	return in * scale_y + translate_y;
}

point transformation::apply(point in)
{
	return point{ apply_x(in.x), apply_y(in.y) };
}
