#include "pch.h"
#include "../editor-lib/command_add_curve.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

namespace test_commands
{
	TEST_CLASS(add_curve)
	{
	public:

		TEST_METHOD(add_curve_increases_curve_count)
		{
			auto dom = document_model();
			auto count_before = dom.curve_list.size();
			auto cmd = commands::add_curve(dom);
			cmd.exec();
			auto count_after_exec = dom.curve_list.size();
			cmd.undo();
			auto count_after_undo = dom.curve_list.size();
			Assert::AreEqual(count_before, count_after_undo);
			Assert::AreEqual(count_before + 1, count_after_exec);
		}
	};
}