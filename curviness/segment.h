#pragma once
#include <vector>
#include "algorithms.h"

struct segment
{
	float eval() const;
	float eval(float normalized_time) const;
	float eval(int min, int time, int max) const;
	float eval(float min, float time, float max) const;

	std::vector<float> params = { .0f };
	float(*algorithm)(float, const float* params, size_t count) = algorithms::generalized_bezier;
};