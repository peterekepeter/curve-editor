#pragma once

namespace algorithms
{
	namespace basic
	{
		float linear_interpolation(float t, float a, float b);

		float quadratic_bezier(float t, float a, float b, float c);

		float cubic_bezier(float t, float a, float b, float c, float d);

		float generalized_bezier(float t, const float* points, size_t count);
	}

	float generalized_bezier(float t, const float* points, size_t count);
}

struct segment
{

	float eval() const;
	float eval(float normalized_time) const;
	float eval(int min, int time, int max) const;


	std::vector<float> params = { .0f };
	float(*algorithm)(float, const float* params, size_t count) = algorithms::generalized_bezier;

};

struct segment_with_separators
{
	int left, right;
	segment& segment;
	float eval(int time);
};

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
	index_t find_segment_index(int time) const;
	index_t find_separator(int time) const;
	void remove_split(index_t index);
	segment& get_segment_by_index(index_t index);

	const segment& get_segment_by_index(index_t index) const;
	segment_with_separators get_segment(int time);
	float eval(int time);

private:

	segment_list segments = { { { .0f }, algorithms::generalized_bezier } };
	separator_list separators = { };
};
