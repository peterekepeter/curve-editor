#include "pch.h"
#include "../editor-lib/editor.h"
#include "../editor-lib/command_split.h"

namespace lib
{
	TEST_CLASS(test_editor)
	{
	public:

		TEST_METHOD(registers_and_executes_commands)
		{
			editor editor;
			editor.document.curve_list.emplace_back();
			auto& curve = editor.document.curve_list[0];

			editor.history.commit(std::make_unique
				<commands::split>(editor.document, 0, 13.0));
			editor.history.commit(std::make_unique
				<commands::split>(editor.document, 0, 42.0));

			Assert::AreEqual(size_t(0), 
				curve.find_segment_index(10.0));
			Assert::AreEqual(size_t(1), 
				curve.find_segment_index(15.0));
			Assert::AreEqual(size_t(2), 
				curve.find_segment_index(64.0));
		}

		TEST_METHOD(commands_can_be_undone)
		{
			editor editor;
			editor.document.curve_list.emplace_back();
			auto& curve = editor.document.curve_list[0];

			editor.history.commit(std::make_unique
				<commands::split>(editor.document, 0, 13.0));
			editor.history.commit(std::make_unique
				<commands::split>(editor.document, 0, 42.0));

			editor.history.undo();
			editor.history.undo();

			Assert::AreEqual(size_t(0),
				curve.find_segment_index(10.0));
			Assert::AreEqual(size_t(0),
				curve.find_segment_index(15.0));
			Assert::AreEqual(size_t(0),
				curve.find_segment_index(64.0));
		}

		TEST_METHOD(commands_can_be_redone)
		{
			editor editor;
			editor.document.curve_list.emplace_back();
			auto& curve = editor.document.curve_list[0];

			editor.history.commit(std::make_unique
				<commands::split>(editor.document, 0, 13.0));
			editor.history.commit(std::make_unique
				<commands::split>(editor.document, 0, 42.0));

			editor.history.undo();
			editor.history.undo();

			editor.history.redo();
			editor.history.redo();

			Assert::AreEqual(size_t(0),
				curve.find_segment_index(10.0));
			Assert::AreEqual(size_t(1),
				curve.find_segment_index(15.0));
			Assert::AreEqual(size_t(2),
				curve.find_segment_index(64.0));
		}
		
		TEST_METHOD(unro_redo_limits)
		{
			editor editor;
			editor.document.curve_list.emplace_back();

			Assert::IsFalse(editor.history.undo());
			Assert::IsFalse(editor.history.redo());

			editor.history.commit(std::make_unique
				<commands::split>(editor.document, 0, 13.0));
			
			Assert::IsTrue(editor.history.undo());
			Assert::IsFalse(editor.history.undo());
			Assert::IsFalse(editor.history.undo());

			Assert::IsTrue(editor.history.redo());
			Assert::IsFalse(editor.history.redo());
			Assert::IsFalse(editor.history.redo());
		}
	};
}