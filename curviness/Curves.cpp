#include <functional>
#include <vector>
#include "Curves.h"

float algorithms::basic::linear_interpolation(float t, float a, float b) {
	return a + t * (b - a);
}

float algorithms::basic::quadratic_bezier(float t, float a, float b, float c) {
	float one_minus_t = 1 - t;
	return a * one_minus_t*one_minus_t + b * 2 * one_minus_t * t + c * t*t;
}

float algorithms::basic::cubic_bezier(float t, float a, float b, float c, float d) {
	float one_minus_t = 1 - t;
	float one_minus_t_sqr = one_minus_t * one_minus_t;
	float t_sqr = t * t;
	return  a * one_minus_t_sqr*one_minus_t + 3 * b*one_minus_t_sqr * t + 3 * c*one_minus_t*t_sqr + d * t_sqr*t;
}

float algorithms::basic::generalized_bezier(float t, const float * points, size_t count)
{
	switch (count)
	{
	case 0: return 0.0f; // makes no sense
	case 1: return *points;
	case 2: return linear_interpolation(t, points[0], points[1]);
	case 3: return quadratic_bezier(t, points[0], points[1], points[2]);
	case 4: return cubic_bezier(t, points[0], points[1], points[2], points[3]);
	case 5:	return linear_interpolation(t,
		generalized_bezier(t, points, count - 1),
		generalized_bezier(t, points + 1, count - 1));
	case 6:	return quadratic_bezier(t,
		generalized_bezier(t, points, count - 2),
		generalized_bezier(t, points + 1, count - 2),
		generalized_bezier(t, points + 2, count - 2));
	default:
	case 7:	return cubic_bezier(t,
		generalized_bezier(t, points, count - 3),
		generalized_bezier(t, points + 1, count - 3),
		generalized_bezier(t, points + 2, count - 3),
		generalized_bezier(t, points + 3, count - 3));
	}

}

float algorithms::generalized_bezier(float t, const float * points, size_t count)
{
	return basic::generalized_bezier(t, points, count);
}

curve::index_t curve::split(int time)
{
	// before everything split does consitency check
	if (this->separators.size() != this->segments.size() + 1)
	{
		while (this->separators.size() >= this->segments.size() ) {
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

size_t curve::find_segment_index(int time) const
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

segment& curve::find_segment(int time) {
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

segment_with_separators curve::get_segment(int time)
{
	auto index = find_segment_index(time);
	if (index <= 0) {
		auto& segment = get_segment_by_index(index);
		return segment_with_separators{ 0, 0, segment };
	}
	auto sep_before_index = index - 1;
	auto sep_after_index = index;
	auto last = separators.size() - 1;
	if (sep_after_index > last) { sep_after_index = last; }
	if (sep_after_index <= sep_before_index) {
		auto sep = separators[sep_after_index];
		auto& segment = get_segment_by_index(index);
		return segment_with_separators{ sep, sep, segment };
	}
	auto sep_before = separators[sep_before_index];
	auto sep_after = separators[sep_after_index];
	auto& segment = get_segment_by_index(index);
	return segment_with_separators { sep_before, sep_after, segment };
}

float curve::eval(int time)
{
	return this->get_segment(time).eval(time);
}

float segment::eval() const
{
	return this->eval(.5f);
}

float segment::eval(float normalized_time) const {
	return this->algorithm(normalized_time, this->params.data(), current_count);
}

float segment::eval(int min, int time, int max) const
{
	return this->eval((time - min) / float(max - min));
}

float segment_with_separators::eval(int time)
{
	if (this->left >= this->right) {
		return this->segment.eval();
	}
	return this->segment.eval(this->left, time, this->right);
}
