#include "curve.h"

curve::index_t curve::split(time_t time)
{
	// before everything split does consitency check
	if (this->separators.size() != this->segments.size() + 1)
	{
		while (this->separators.size() >= this->segments.size()) {
			segment new_segment;
			new_segment.params.push_back(.0f);
			new_segment.algorithm = algorithms::generalized_bezier;
			this->segments.push_back(new_segment);
		}
	}

	// add new separator
	index_t segment_index = this->find_segment_index(time);
	index_t insert_index = 0;
	bool did_insert = false;
	for (auto pos = this->separators.begin(); pos != this->separators.end(); pos++) {
		if (*pos > time) {
			this->separators.insert(pos, time);
			did_insert = true;
			break;
		}
		insert_index++;
	}
	if (!did_insert) {
		this->separators.push_back(time);
	}

	// add new segment
	auto pos = this->segments.begin() + segment_index;
	auto& segment = *pos;
	auto new_segment = segment; //copy
	this->segments.insert(pos, new_segment);
	return insert_index;
}

size_t curve::find_segment_index(time_t time) const
{
	size_t begin = 0, end = separators.size();
	while (begin < end) {
		size_t middle = (begin + end) / 2;
		if (time < separators[middle]) {
			end = middle;
		}
		else {
			begin = middle + 1;
		}
	}
	return begin;
}

curve::index_t curve::find_separator_index(time_t time) const
{
	size_t separator_count = separators.size();
	if (separator_count == 0)
	{
		throw "has no separators";
	}
	if (separator_count == 1) 
	{
		return 0;
	}
	size_t segment_index = find_segment_index(time);
	//  0   1   2
	// ---|---|---
	//    0   1
	if (segment_index == 0) 
	{
		return segment_index;
	}
	if (segment_index == segments.size() - 1) 
	{
		return segment_index - 1;
	}
	if (time - separators[segment_index - 1] < separators[segment_index] - time)
	{
		return segment_index - 1;
	}
	else 
	{
		return segment_index;
	}
}

void curve::remove_split(index_t index)
{
	segments.erase(segments.begin() + index);
	separators.erase(separators.begin() + index);
}

segment& curve::find_segment_ref(time_t time) {
	return get_segment_ref_by_index(find_segment_index(time));
}

void curve::remove_zero_length_segments()
{
	bool continue_checking = true;
	while (continue_checking) {
		continue_checking = false;
		for (size_t i = 1; i < separators.size(); i++) {
			if (separators[i - 1] != separators[i]) {
				continue;
			}
			segments.erase(segments.begin() + i);
			separators.erase(separators.begin() + i);
			continue_checking = true;
			break;
		}
	}
}

segment& curve::get_segment_ref_by_index(curve::index_t index)
{
	return this->segments[index];
}

segment_with_separators<curve::time_t> curve::get_segment_by_index(index_t segment_index)
{
	auto separators_count = separators.size();

	auto before_index = segment_index - 1;
	auto after_index = segment_index;

	time_t sep_before = -INFINITY;
	if (0 <= before_index && before_index < separators_count)
	{
		sep_before = separators[before_index];
	}

	time_t sep_after = +INFINITY;
	if (0 <= after_index && after_index < separators_count)
	{
		sep_after = separators[after_index];
	}

	auto& segment = get_segment_ref_by_index(segment_index);
	return segment_with_separators<curve::time_t>
	{
		sep_before,
		sep_after,
		segment,
		segment_index
	};
}


const segment& curve::get_segment_ref_by_index(curve::index_t index) const
{
	return this->segments[index];
}

const segment_with_separators_c<curve::time_t> curve::get_segment_by_index(index_t segment_index) const
{
	auto separators_count = separators.size();

	auto before_index = segment_index - 1;
	auto after_index = segment_index;

	time_t sep_before = -INFINITY;
	if (0 <= before_index && before_index < separators_count)
	{
		sep_before = separators[before_index];
	}

	time_t sep_after = +INFINITY;
	if (0 <= after_index && after_index < separators_count)
	{
		sep_after = separators[after_index];
	}

	auto& segment = get_segment_ref_by_index(segment_index);
	return segment_with_separators_c<curve::time_t>
	{
		sep_before,
		sep_after,
		segment,
		segment_index
	};
}

segment_with_separators<curve::time_t> curve::find_segment(curve::time_t time)
{
	auto segment_index = find_segment_index(time);
	return get_segment_by_index(segment_index);
}

segment_with_separators_c<curve::time_t> curve::find_segment(curve::time_t time) const
{
	auto segment_index = find_segment_index(time);
	return get_segment_by_index(segment_index);
}

float curve::eval(time_t time) const
{
	return this->find_segment(time).eval(time);
}

curve::time_t curve::get_separator_value(index_t index) const
{
	return separators[index];
}

void curve::set_separator_value(index_t index, time_t value)
{
	time_t min_val = -INFINITY;
	time_t max_val = +INFINITY;
	if (index > 0) {
		min_val = separators[index - 1];
	}
	if (index + 1 < separators.size()) {
		max_val = separators[index + 1];
	}
	// clamp
	if (value < min_val) {
		value = min_val;
	}
	if (value > max_val) {
		value = max_val;
	}
	separators[index] = value;
}

size_t curve::get_separator_count() const
{
	return separators.size();
}
