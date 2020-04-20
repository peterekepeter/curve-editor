#include "pch.h"
#include "../editor-lib/command_edit_param.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

namespace test_commands
{
	TEST_CLASS(edit_param_command)
	{
	public:

		using edit_param = commands::edit_param;

		TEST_METHOD(can_edit_parameter)
		{
			// arrange
			auto document = setup_document();
			auto& curve = document.curve_list[0];
			Assert::AreEqual(curve.eval(0.0), 0.0f, 1e-3f);

			// act
			edit_param(document, 0, 1, 0, 1.0f).exec();

			// assert
			Assert::AreEqual(curve.eval(0.0), 1.0f, 1e-3f);
		}

		TEST_METHOD(edit_can_be_undone)
		{
			// arrange
			auto document = setup_document();
			auto& curve = document.curve_list[0];
			auto cmd = edit_param(document, 0, 1, 0, 1.0f);
			cmd.exec();
			Assert::AreEqual(curve.eval(0.0), 1.0f, 1e-3f);

			// act
			cmd.undo();

			// assert
			Assert::AreEqual(curve.eval(0.0), 0.0f, 1e-3f);
		}

		document_model setup_document() {
			auto document = document_model();
			document.curve_list.emplace_back();
			auto& curve = document.curve_list[0];
			curve.split(0.0);
			curve.split(1.0);
			auto& segment = curve.find_segment(0.5);
			segment.params = { 0.0f, 1.0f, 1.0f, 0.0f };
			return document;
		}
	};
}