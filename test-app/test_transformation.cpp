#include "pch.h"
#include "../editor-lib/transformation.h"

namespace utils
{
	TEST_CLASS(tranformation)
	{
	public:

		TEST_METHOD(transformation_structured_init)
		{
			auto t = transformation{ 1, 2, 3, 4 };

			Assert::AreEqual(1, t.scale_x, 1e-4f);
			Assert::AreEqual(2, t.scale_y, 1e-4f);
			Assert::AreEqual(3, t.translate_x, 1e-4f);
			Assert::AreEqual(4, t.translate_y, 1e-4f);
		}

		TEST_METHOD(transformation_is_applied_correctly)
		{
			auto t = transformation{ 2, 3, .5f, .75f };
			Assert::AreEqual(2.50f, t.apply_x(1), 1e-4f);
			Assert::AreEqual(6.75f, t.apply_y(2), 1e-4f);
		}

		TEST_METHOD(transformation_can_be_applied_to_point)
		{
			auto t = transformation{ 2, 3, .5f, .75f };
			auto p = point{ 1, 1 };
			auto p2 = t.apply(p);
			Assert::AreEqual(2.50f, p2.x, 1e-4f);
			Assert::AreEqual(3.75f, p2.y, 1e-4f);
		}

		TEST_METHOD(transformation_can_be_inverted)
		{
			auto t = transformation{ 2, 3, .5f, .75f };
			auto p = point{ 1, 1 };
			auto p2 = t.apply(p);
			auto p3 = t.inverse().apply(p2);
			Assert::AreNotEqual(p.x, p2.x, 1e-4f);
			Assert::AreNotEqual(p.y, p2.y, 1e-4f);
			Assert::AreEqual(p.x, p3.x, 1e-4f);
			Assert::AreEqual(p.y, p3.y, 1e-4f);
		}

	};
}
