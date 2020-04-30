#pragma once
#include "segment.h"

template <class time_t>
struct segment_with_separators
{
	time_t left, right;
	segment& segment;
	size_t segment_index;
	float eval(time_t time) const;
};

template <class time_t>
struct segment_with_separators_c
{
	time_t left, right;
	const segment& segment;
	size_t segment_index;
	float eval(time_t time) const;
};

template<class time_t>
inline float segment_with_separators<time_t>::eval(time_t time) const
{
	return segment.eval(left, time, right);
}

template<class time_t>
inline float segment_with_separators_c<time_t>::eval(time_t time) const
{
	return segment.eval(left, time, right);
}
