#include "pch.h"
#include "../editor-ui-lib/curve_editor.h"
#include "../editor-ui-lib/edit_control_y_param.h"

namespace lib
{
	TEST_CLASS(test_curve_editor)
	{
	public:

		TEST_METHOD(get_nearest_edit_control_empty_model)
		{
			document_model model;
			curve_editor editor{ model };
			check_selection_valid(editor, 0, false);
			auto result = editor.get_nearest_edit_control(0, 0, transformation::identity());
			auto control_ptr = result.control.get();
			Assert::IsNull(control_ptr, L"no control is found on empty model");
		}

		TEST_METHOD(get_nearest_edit_control_model_with_one_curve)
		{
			document_model model;
			curve_editor editor{ model };
			check_selection_valid(editor, 0, false);
			model.curve_list.emplace_back();
			auto result = editor.get_nearest_edit_control(0, 0, transformation::identity());
			auto control_ptr = result.control.get();
			Assert::IsTrue(control_ptr != nullptr, L"valid control is returned");
			auto y_control = dynamic_cast<edit_control_y_param*>(control_ptr);
			Assert::IsNotNull(y_control, L"a y_param control shoul be returned");
		}

		TEST_METHOD(valids_selection)
		{
			document_model model;
			curve_editor editor{ model };
			check_selection_valid(editor, 0, false);
			model.curve_list.emplace_back();
			check_selection_valid(editor, 0, true);
			check_selection_valid(editor, 1, false);
			check_selection_valid(editor, -1, false);
		}

		void check_selection_valid(curve_editor& editor, size_t index, bool expected) {
			editor.curve_index = index;
			Assert::AreEqual(expected, editor.is_selection_valid());
		}
	};
}