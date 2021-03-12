#include "algorithms.h"

float algorithms::basic::linear_interpolation(float t, float a, float b) {
	return a + t * (b - a);
}

float algorithms::basic::quadratic_bezier(float t, float a, float b, float c) {
	float one_minus_t = 1 - t;
	return a * one_minus_t * one_minus_t + b * 2 * one_minus_t * t + c * t * t;
}

float algorithms::basic::cubic_bezier(float t, float a, float b, float c, float d) {
	float one_minus_t = 1 - t;
	float one_minus_t_sqr = one_minus_t * one_minus_t;
	float t_sqr = t * t;
	return  a * one_minus_t_sqr * one_minus_t + 3 * b * one_minus_t_sqr * t + 3 * c * one_minus_t * t_sqr + d * t_sqr * t;
}

float algorithms::basic::generalized_bezier(float t, const float* points, size_t count)
{
	switch (count)
	{
	case 0: return 0.0f; // makes no sense
	case 1: return *points;
	case 2: return linear_interpolation(t, points[0], points[1]);
	case 3: return quadratic_bezier(t, points[0], points[1], points[2]);
	case 4: return cubic_bezier(t, points[0], points[1], points[2], points[3]);
	case 5:	return linear_interpolation(t,
		generalized_bezier(t, points, count - 1),
		generalized_bezier(t, points + 1, count - 1));
	case 6:	return quadratic_bezier(t,
		generalized_bezier(t, points, count - 2),
		generalized_bezier(t, points + 1, count - 2),
		generalized_bezier(t, points + 2, count - 2));
	default:
	case 7:	return cubic_bezier(t,
		generalized_bezier(t, points, count - 3),
		generalized_bezier(t, points + 1, count - 3),
		generalized_bezier(t, points + 2, count - 3),
		generalized_bezier(t, points + 3, count - 3));
	}

}

float algorithms::generalized_bezier(float t, const float* points, size_t count)
{
	return basic::generalized_bezier(t, points, count);
}
