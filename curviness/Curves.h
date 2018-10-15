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

	float generalized_bezier(float t, const std::vector<float>& data);
}

struct segment
{
	std::function<float(float, const std::vector<float>&)> algorithm;
	std::vector<float> params;
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
	index_t find_segment(int time) const;
	index_t find_separator(int time) const;
	void remove_split(index_t index);
	segment& get_segment(index_t index);
	const segment& get_segment(index_t index) const;

private:

	segment_list segments;
	separator_list separators;
};
