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

	// search aglorithms
	segment& find_segment_ref(time_t);
	segment_with_separators<time_t> find_segment(time_t);
	segment_with_separators_c<time_t> find_segment(time_t) const;
	index_t find_segment_index(time_t) const;
	index_t find_separator_index(time_t) const;

	void remove_zero_length_segments();
	segment& get_segment_ref_by_index(index_t segment_index);
	segment_with_separators<time_t> get_segment_by_index(index_t segment_index);

	const segment& get_segment_ref_by_index(index_t index) const;
	const segment_with_separators_c<time_t> get_segment_by_index(index_t index) const;
	float eval(time_t time) const;

	void remove_split(index_t separator_index);
	time_t get_separator_value(index_t index) const;
	void set_separator_value(index_t index, time_t value);
	size_t get_separator_count() const;
	size_t get_segment_count() const { return segments.size(); }

private:

	segment_list segments = { { { .0f }, algorithms::generalized_bezier } };
	separator_list separators = { };
};
