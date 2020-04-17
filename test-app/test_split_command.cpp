#include "pch.h"
#include "../editor-lib/command_split.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

namespace test_commands
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

			check_segment_index(curve, 16.0, 0);

			std::unique_ptr<commands::base> command
				= std::make_unique<commands::split>(document, 0, 14.0);
			
			// act
			command->exec();

			// assert
			check_segment_index(curve, 16.0, 1);
		}

		TEST_METHOD(split_can_be_undone)
		{
			// arrange
			auto document = document_model();
			document.curve_list.emplace_back();
			auto& curve = document.curve_list[0];
			std::unique_ptr<commands::base> command
				= std::make_unique<commands::split>(document, 0, 14.0);
			command->exec();
			check_segment_index(curve, 16.0, 1);

			// act
			command->undo();

			check_segment_index(curve, 16.0, 0);
		}
	};
}