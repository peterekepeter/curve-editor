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

segment& curve::find_segment(time_t time) {
	return get_segment_by_index(find_segment_index(time));
}

segment& curve::get_segment_by_index(curve::index_t index)
{
	return this->segments[index];
}

const segment& curve::get_segment_by_index(curve::index_t index) const
{
	return this->segments[index];
}

segment_with_separators<curve::time_t> curve::get_segment(curve::time_t time)
{
	auto index = find_segment_index(time);
	if (index <= 0) {
		auto& segment = get_segment_by_index(index);
		return segment_with_separators<time_t>{ 0, 0, segment };
	}
	auto sep_before_index = index - 1;
	auto sep_after_index = index;
	auto last = separators.size() - 1;
	if (sep_after_index > last) { sep_after_index = last; }
	if (sep_after_index <= sep_before_index) {
		auto sep = separators[sep_after_index];
		auto& segment = get_segment_by_index(index);
		return segment_with_separators<time_t>{ sep, sep, segment };
	}
	auto sep_before = separators[sep_before_index];
	auto sep_after = separators[sep_after_index];
	auto& segment = get_segment_by_index(index);
	return segment_with_separators<time_t>{ sep_before, sep_after, segment };
}

float curve::eval(time_t time)
{
	return this->get_segment(time).eval(time);
}