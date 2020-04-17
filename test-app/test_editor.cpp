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
			document_editor document_editor;
			document_editor.document.curve_list.emplace_back();
			auto& curve = document_editor.document.curve_list[0];

			document_editor.commit(std::make_unique
				<command::split>(document_editor.document, 0, 13.0));
			document_editor.commit(std::make_unique
				<command::split>(document_editor.document, 0, 42.0));

			Assert::AreEqual(size_t(0), 
				curve.find_segment_index(10.0));
			Assert::AreEqual(size_t(1), 
				curve.find_segment_index(15.0));
			Assert::AreEqual(size_t(2), 
				curve.find_segment_index(64.0));
		}

		TEST_METHOD(commands_can_be_undone)
		{
			document_editor document_editor;
			document_editor.document.curve_list.emplace_back();
			auto& curve = document_editor.document.curve_list[0];

			document_editor.commit(std::make_unique
				<command::split>(document_editor.document, 0, 13.0));
			document_editor.commit(std::make_unique
				<command::split>(document_editor.document, 0, 42.0));

			document_editor.undo();
			document_editor.undo();

			Assert::AreEqual(size_t(0),
				curve.find_segment_index(10.0));
			Assert::AreEqual(size_t(0),
				curve.find_segment_index(15.0));
			Assert::AreEqual(size_t(0),
				curve.find_segment_index(64.0));
		}

		TEST_METHOD(commands_can_be_redone)
		{
			document_editor document_editor;
			document_editor.document.curve_list.emplace_back();
			auto& curve = document_editor.document.curve_list[0];

			document_editor.commit(std::make_unique
				<command::split>(document_editor.document, 0, 13.0));
			document_editor.commit(std::make_unique
				<command::split>(document_editor.document, 0, 42.0));

			document_editor.undo();
			document_editor.undo();

			document_editor.redo();
			document_editor.redo();

			Assert::AreEqual(size_t(0),
				curve.find_segment_index(10.0));
			Assert::AreEqual(size_t(1),
				curve.find_segment_index(15.0));
			Assert::AreEqual(size_t(2),
				curve.find_segment_index(64.0));
		}
	};
}