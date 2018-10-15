#include "stdafx.h"
#include "../curviness/Curves.h"

namespace curviness_test
{
	constexpr auto lerp = algorithms::basic::linear_interpolation;

	TEST_CLASS(Algorithms)
	{
	public:
		
		TEST_METHOD(LinearInterpolationBasicTest)
		{
			Assert::AreEqual(0.5f, lerp(.5f, 0.0f, 1.0f), 0.0001f);
		}

	};
}