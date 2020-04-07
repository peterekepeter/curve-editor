#include "stdafx.h"
#include "../curviness/curviness.h"

namespace curviness_test
{
	constexpr auto lerp = algorithms::basic::linear_interpolation;

	TEST_CLASS(algorithms)
	{
	public:
		
		TEST_METHOD(linear_interpolation_basic_test)
		{
			Assert::AreEqual(0.5f, lerp(.5f, 0.0f, 1.0f), 0.0001f);
		}

	};
}