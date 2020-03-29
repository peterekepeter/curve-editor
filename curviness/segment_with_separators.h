#pragma once
#include "segment.h"

template <class time_t>
struct segment_with_separators
{
	time_t left, right;
	segment& segment;
	float eval(time_t time);
};

template<class time_t>
inline float segment_with_separators<time_t>::eval(time_t time)
{
	return segment.eval(left, time, right);
}
