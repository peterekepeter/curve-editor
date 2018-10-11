#include "stdafx.h"
#include "CppUnitTest.h"

#include "../curviness/Curves.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	constexpr auto lerp = algorithms::linear_interpolation;

	TEST_CLASS(Algorithms)
	{
	public:
		
		TEST_METHOD(LinearInterpolationBasicTest)
		{
			// TODO: Your test code here
			Assert::AreEqual(0.5f, lerp(.5f, 0.0f, 1.0f), 0.0001f);
		}

	};
}