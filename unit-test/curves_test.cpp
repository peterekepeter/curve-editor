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
			Assert::AreNotEqual(curve.find_segment_index(59), curve.find_segment_index(61));
		}

		TEST_METHOD(MultipleSplitsGeneratePredictableSegmentIndexes)
		{
			curve curve;
			curve.split(10);
			curve.split(30);
			curve.split(20);
			Assert::AreEqual(curve::index_t(0), curve.find_segment_index(5));
			Assert::AreEqual(curve::index_t(1), curve.find_segment_index(15));
			Assert::AreEqual(curve::index_t(2), curve.find_segment_index(25));
			Assert::AreEqual(curve::index_t(3), curve.find_segment_index(35));
		}

		TEST_METHOD(CanGetSegment) {
			curve curve;
			curve.split(10);
			curve.split(20);
			auto& segment = curve.get_segment(curve.find_segment_index(15));
		}

		TEST_METHOD(CanGetSegmentByTime) {
			curve curve; curve.split(10);
			auto& segment = curve.find_segment(11);
			segment.algorithm = algorithms::generalized_bezier;
			auto& segment2 = curve.find_segment(12);
			Assert::IsTrue(segment2.algorithm == algorithms::generalized_bezier);
		}

		TEST_METHOD(CanEvaluateSegment) {
			segment segment = { algorithms::generalized_bezier, { 1.0f, 2.0f } };
			Assert::AreEqual(1.5f, segment.eval(0.5f));
		}

		TEST_METHOD(CurveStartsWithConstantValueSegment) {
			curve curve;
			auto& segment = curve.find_segment(0);
			Assert::AreEqual(0.0f, segment.eval(0.0f));
		}

		TEST_METHOD(CanEvaluateFreslyCreatedCurve) {
			Assert::AreEqual(0.0f, curve().eval(14));
		}

		TEST_METHOD(CanEvaluateCurveBeyondLastSplitPosition) {
			curve curve;
			curve.split(40);
			curve.eval(45);
		}

		TEST_METHOD(CanEvaluateCurveBeforeFirstSplitPosition) {
			curve curve;
			curve.split(44);
			curve.split(-5);
			curve.eval(-10);
		}

		TEST_METHOD(CanEvaluateCurveWithSomeValues) {
			curve curve;
			curve.split(10);
			curve.split(20);
			auto& segment = curve.find_segment(15);
			segment.params = { 2.0f, 3.0f };
			Assert::AreEqual(2.5f, curve.eval(15));
		}
	};
}