#pragma once
#include <vector>
#include "algorithms.h"

struct segment
{
	float eval() const;
	float eval(float normalized_time) const;

	template<class min_t, class time_t, class max_t> 
	float eval(min_t min, time_t time, max_t max) const;

	std::vector<float> params = { .0f };

	float(*algorithm)(float, const float* params, size_t count) 
		= algorithms::generalized_bezier;
};

template<class min_t, class time_t, class max_t>
inline float segment::eval(min_t min, time_t time, max_t max) const
{
	return this->eval(
		float(time - min) / 
		float(max - min));
}
