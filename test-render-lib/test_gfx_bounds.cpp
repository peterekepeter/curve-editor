#include "pch.h"
#include "../render-lib/puha-render.h"

namespace lib
{
	TEST_CLASS(test_rect_fill)
	{
	public:

		TEST_METHOD(without_viewport_goes_over_boundaries)
		{
			Gfx320x200 gfx;
			gfx.RectFill(-1, -1, 3, 3, 0xff0000);
			Assert::IsFalse(gfx.AreBoundsValid());
		}

		TEST_METHOD(top_is_clipped)
		{
			Gfx320x200 gfx;
			auto view = gfx.CreateViewport();
			view.RectFill(gfx.Width/2, -2, 5, 5, 0xff0000);
			Assert::IsTrue(gfx.AreBoundsValid());
		}

		TEST_METHOD(bottom_is_clipped)
		{
			Gfx320x200 gfx;
			auto view = gfx.CreateViewport();
			view.RectFill(gfx.Width/2, gfx.Height-1, 3, 3, 0xff0000);
			Assert::IsTrue(gfx.AreBoundsValid());
		}
	};
}