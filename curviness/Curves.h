#pragma once

namespace algorithms
{
	float linear_interpolation(float t, float a, float b);

	float quadratic_bezier(float t, float a, float b, float c);

	float cubic_bezier(float t, float a, float b, float c, float d);

	float generalized_bezier(float t, const float* points, size_t count);

	float generalized_bezier(float t, const std::vector<float>& data);
}

struct segment
{
	std::function<float(std::vector<float>)> algorithm;
	std::vector<float> params;
};

struct curve
{
	using segment_list = std::vector<segment>;
	using separator_list = std::vector<unsigned>;
	segment_list segments;
	separator_list separators;
};
