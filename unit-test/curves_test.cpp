#include "stdafx.h"
#include "../curviness/Curves.h"

namespace curviness_test
{
	TEST_CLASS(Curves)
	{
	public:

		TEST_METHOD(SplitCreatesNewSegments)
		{
			curve curve;
			curve.split(60);
			Assert::AreNotEqual(curve.find_segment(59), curve.find_segment(61));
		}

		TEST_METHOD(MultipleSplitsGeneratePredictableSegmentIndexes)
		{
			curve curve;
			curve.split(10);
			curve.split(30);
			curve.split(20);
			Assert::AreEqual(curve::index_t(0), curve.find_segment(5));
			Assert::AreEqual(curve::index_t(1), curve.find_segment(15));
			Assert::AreEqual(curve::index_t(2), curve.find_segment(25));
			Assert::AreEqual(curve::index_t(3), curve.find_segment(35));
		}

		TEST_METHOD(CanGetSegment) {
			curve curve;
			curve.split(10);
			curve.split(20);
			auto& segment = curve.get_segment(curve.find_segment(15));
		}
	};
}