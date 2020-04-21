#include "pch.h"
#include "../editor-lib/command_edit_separator.h"
#include <initializer_list>

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

namespace test_commands
{
	TEST_CLASS(edit_separator_command)
	{
	public:

		using edit_separator = commands::edit_separator;

		TEST_METHOD(can_move_separator_when_only_one)
		{
			// arrange
			auto document = document_with_splits({ 14.0 });
			auto& curve = document.curve_list[0];
			check_segment_index(curve, 27.9, 1);

			// act
			edit_separator(document, 0, 14.0, 28.0).exec();

			// assert
			check_segment_index(curve, 27.9, 0);
		}

		TEST_METHOD(move_can_be_undone_when_only_one_separator)
		{
			// arrange
			auto document = document_with_splits({ 14.0 });
			auto& curve = document.curve_list[0];
			auto command = edit_separator(document, 0, 14.0, 28.0);
			command.exec();
			check_segment_index(curve, 27.0, 0);

			// act
			command.revert();

			// assert
			check_segment_index(curve, 27.0, 1);
		}

		TEST_METHOD(squashed_segment_is_removed_when_moving_right)
		{
			// arrange
			auto document = document_with_splits({ 3.0, 4.0 });
			auto& curve = document.curve_list[0];
			Assert::AreEqual(size_t(2), curve.get_separator_count());

			// act
			edit_separator(document, 0, 3.0, 4.0).exec();

			// assert
			Assert::AreEqual(size_t(1), curve.get_separator_count());
		}

		TEST_METHOD(squashed_segment_is_removed_when_moving_left)
		{
			// arrange
			auto document = document_with_splits({ 3.0, 4.0 });
			auto& curve = document.curve_list[0];
			Assert::AreEqual(size_t(2), curve.get_separator_count());

			// act
			edit_separator(document, 0, 4.0, 3.0).exec();

			// assert
			Assert::AreEqual(size_t(1), curve.get_separator_count());
		}

		TEST_METHOD(squashed_segment_can_be_restored_when_moving_right)
		{
			// arrange
			auto document = document_with_splits({ 3.0, 4.0 });
			auto& curve = document.curve_list[0];
			auto cmd = edit_separator(document, 0, 3.0, 4.0);
			cmd.exec();
			Assert::AreEqual(size_t(1), curve.get_separator_count());

			// act
			cmd.revert();

			// assert
			Assert::AreEqual(size_t(2), curve.get_separator_count());
		}

		TEST_METHOD(squashed_segment_can_be_restored_when_moving_left)
		{
			// arrange
			auto document = document_with_splits({ 3.0, 4.0 });
			auto& curve = document.curve_list[0];
			auto cmd = edit_separator(document, 0, 4.0, 3.0);
			cmd.exec();
			Assert::AreEqual(size_t(1), curve.get_separator_count());

			// act
			cmd.revert();

			// assert
			Assert::AreEqual(size_t(2), curve.get_separator_count());
		}

		TEST_METHOD(correct_segment_is_removed_when_squashing_to_right)
		{
			// arrange
			auto document = document_with_2_splits();
			size_t curve_index = 0;
			auto& curve = document.curve_list[curve_index];
			auto cmd = edit_separator(document, curve_index, 1.0, 2.0);
			Assert::AreEqual(1.0f, curve.eval(1.5), 1e-6f);

			// act
			cmd.exec();

			// assert
			Assert::AreEqual(0.0f, curve.eval(1.5), 1e-6f);
		}

		TEST_METHOD(correct_segment_is_removed_when_squashing_to_left)
		{
			// arrange
			auto document = document_with_2_splits();
			size_t curve_index = 0;
			auto& curve = document.curve_list[curve_index];
			auto cmd = edit_separator(document, curve_index, 2.0, 1.0);
			Assert::AreEqual(1.0f, curve.eval(1.5), 1e-6f);

			// act
			cmd.exec();

			// assert
			Assert::AreEqual(0.0f, curve.eval(1.5), 1e-6f);
		}

		document_model document_with_splits(std::initializer_list<double> splits) {
			auto document = document_model();
			document.curve_list.emplace_back();
			auto& curve = document.curve_list[0];
			for (auto split : splits) {
				curve.split(split);
			}
			return document;
		}

		document_model document_with_2_splits() {
			auto document = document_with_splits({1.0, 2.0});
			auto& middle = document.curve_list[0].find_segment(1.5);
			middle.params[0] = 1.0f;
			return document;
		}
	};
}