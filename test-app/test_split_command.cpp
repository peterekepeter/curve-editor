#include "pch.h"
#include "../editor-lib/command_split.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

namespace commands
{
	TEST_CLASS(split)
	{
	public:

		TEST_METHOD(adds_split_to_curve)
		{
			// arrange
			auto document = document_model();
			document.curve_list.emplace_back();
			auto& curve = document.curve_list[0];

			Assert::AreEqual(size_t(0), curve.find_segment_index(12.0));
			Assert::AreEqual(size_t(0), curve.find_segment_index(16.0));

			std::unique_ptr<command::base> command
				= std::make_unique<command::split>(document, 0, 14.0);
			
			// act
			command->exec();

			// assert
			Assert::AreEqual(size_t(0), curve.find_segment_index(12.0));
			Assert::AreEqual(size_t(1), curve.find_segment_index(16.0));
		}

		TEST_METHOD(split_can_be_undone)
		{
			// arrange
			auto document = document_model();
			document.curve_list.emplace_back();
			auto& curve = document.curve_list[0];

			std::unique_ptr<command::base> command
				= std::make_unique<command::split>(document, 0, 14.0);

			// act
			command->exec();
			command->undo();

			Assert::AreEqual(size_t(0), curve.find_segment_index(12.0));
			Assert::AreEqual(size_t(0), curve.find_segment_index(16.0));
		}
	};
}