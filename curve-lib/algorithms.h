#pragma once

namespace algorithms
{
	namespace basic
	{
		float linear_interpolation(float t, float a, float b);

		float quadratic_bezier(float t, float a, float b, float c);

		float cubic_bezier(float t, float a, float b, float c, float d);

		float generalized_bezier(float t, const float* points, size_t count);
	}

	float generalized_bezier(float t, const float* points, size_t count);
}
