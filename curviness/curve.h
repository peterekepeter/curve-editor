#pragma once
#include <vector>
#include "segment.h"
#include "segment_with_separators.h"
#include "algorithms.h"

struct curve
{
	using index_t = size_t;
	using time_t = double;
	using segment_list = std::vector<segment>;
	using separator_list = std::vector<time_t>;

	index_t split(time_t time);

	segment& find_segment(time_t time);
	segment_with_separators<time_t> get_segment(time_t time);

	index_t find_segment_index(time_t time) const;
	index_t find_separator(time_t time) const;
	void remove_split(index_t index);
	segment& get_segment_by_index(index_t index);

	const segment& get_segment_by_index(index_t index) const;
	float eval(time_t time);

	time_t get_separator_value(index_t index) const;
	void set_separator_value(index_t index, time_t value);

private:

	segment_list segments = { { { .0f }, algorithms::generalized_bezier } };
	separator_list separators = { };
};
