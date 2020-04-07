#include "stdafx.h"
#include "../curviness/curviness.h"

namespace curviness_test
{
	TEST_CLASS(curves)
	{
	public:

		TEST_METHOD(find_separator_returns_nearest_separator)
		{
			curve curve;
			auto a = curve.split(30);
			auto b = curve.split(60);
			Assert::AreEqual(a, curve.find_separator(30));
			Assert::AreEqual(b, curve.find_separator(60));
			Assert::AreEqual(a, curve.find_separator(-200));
			Assert::AreEqual(a, curve.find_separator(40));
			Assert::AreEqual(b, curve.find_separator(50));
			Assert::AreEqual(b, curve.find_separator(999));
			Assert::AreEqual(a, curve.find_separator(-INFINITY));
			Assert::AreEqual(b, curve.find_separator(+INFINITY));
		}

		TEST_METHOD(find_separator_throws_if_no_separators)
		{
			curve curve;
			try 
			{
				curve.find_separator(30);
				Assert::Fail();
			}
			catch (const char* message)
			{

			}
		}

		TEST_METHOD(split_creates_new_segments)
		{
			curve curve;
			curve.split(60);
			Assert::AreNotEqual(curve.find_segment_index(59), curve.find_segment_index(61));
		}

		TEST_METHOD(multiple_splits_generate_predictable_segment_indexes)
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

		TEST_METHOD(can_get_segment) {
			curve curve;
			curve.split(10);
			curve.split(20);
			auto& segment = curve.get_segment_by_index(curve.find_segment_index(15));
		}

		TEST_METHOD(can_get_segment_by_time) {
			curve curve; curve.split(10);
			auto& segment = curve.find_segment(11);
			segment.algorithm = algorithms::generalized_bezier;
			auto& segment2 = curve.find_segment(12);
			Assert::IsTrue(segment2.algorithm == algorithms::generalized_bezier);
		}

		TEST_METHOD(can_evaluate_segment) {
			segment segment = {{ 1.0f, 2.0f }};
			Assert::AreEqual(1.5f, segment.eval(0.5f));
		}

		TEST_METHOD(curve_starts_with_constant_value_segment) {
			curve curve;
			auto& segment = curve.find_segment(0);
			Assert::AreEqual(0.0f, segment.eval(0.0f));
		}

		TEST_METHOD(can_evaluate_fresly_created_curve) {
			Assert::AreEqual(0.0f, curve().eval(14));
		}

		TEST_METHOD(can_evaluate_curve_beyond_last_split_position) {
			curve curve;
			curve.split(40);
			curve.eval(45);
		}

		TEST_METHOD(can_evaluate_curve_before_first_split_position) {
			curve curve;
			curve.split(44);
			curve.split(-5);
			curve.eval(-10);
		}

		TEST_METHOD(can_evaluate_curve_with_some_values) {
			curve curve;
			curve.split(10);
			curve.split(20);
			auto& segment = curve.find_segment(15);
			segment.params = { 2.0f, 3.0f };
			Assert::AreEqual(2.5f, curve.eval(15));
		}
	};
}