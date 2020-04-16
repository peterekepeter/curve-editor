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

			editor.commit(std::make_unique
				<command::split>(editor.document, 0, 13.0));
			editor.commit(std::make_unique
				<command::split>(editor.document, 0, 42.0));

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

			editor.commit(std::make_unique
				<command::split>(editor.document, 0, 13.0));
			editor.commit(std::make_unique
				<command::split>(editor.document, 0, 42.0));

			editor.undo();
			editor.undo();

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

			editor.commit(std::make_unique
				<command::split>(editor.document, 0, 13.0));
			editor.commit(std::make_unique
				<command::split>(editor.document, 0, 42.0));

			editor.undo();
			editor.undo();

			editor.redo();
			editor.redo();

			Assert::AreEqual(size_t(0),
				curve.find_segment_index(10.0));
			Assert::AreEqual(size_t(1),
				curve.find_segment_index(15.0));
			Assert::AreEqual(size_t(2),
				curve.find_segment_index(64.0));
		}
	};
}