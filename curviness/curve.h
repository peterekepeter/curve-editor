#pragma once
#include <vector>
#include "segment.h"
#include "segment_with_separators.h"
#include "algorithms.h"

struct curve
{
	using segment_list = std::vector<segment>;
	using separator_list = std::vector<int>;
	using index_t = size_t;

	struct segment_id
	{
		size_t index;
	};

	struct split_id
	{
		size_t index;
	};

	index_t split(int time);

	segment& find_segment(int time);
	segment_with_separators get_segment(int time);

	index_t find_segment_index(int time) const;
	index_t find_separator(int time) const;
	void remove_split(index_t index);
	segment& get_segment_by_index(index_t index);

	const segment& get_segment_by_index(index_t index) const;
	float eval(int time);

private:

	segment_list segments = { { { .0f }, algorithms::generalized_bezier } };
	separator_list separators = { };
};
