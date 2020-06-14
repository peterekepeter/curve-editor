#include "pch.h"
#include "../editor-lib/io_binary.h"

namespace test_io
{
	TEST_CLASS(test_binary_io)
	{
	public:

		TEST_METHOD(can_write_document_model_to_stringstream)
		{
			std::stringstream s;
			auto doc = create_model();
			io_binary::write(s, doc);
			auto res = s.str();
			Assert::IsTrue(res.size() > 4);
		}

		TEST_METHOD(can_write_and_read_document_model)
		{
			std::stringstream s;
			auto doc = create_model();
			io_binary::write(s, doc);
			auto res = s.str();
			document_model copy;
			io_binary::read(s, copy);
			for (double i = 0; i < 10; i++) {
				Assert::AreEqual(doc.curve_list[0].eval(i), copy.curve_list[0].eval(i));
			}
		}

	private:
		
		document_model create_model() {
			document_model doc;
			doc.curve_list.emplace_back();
			doc.curve_list[0].split(4);
			doc.curve_list[0].split(6);
			auto seg = doc.curve_list[0].find_segment(5);
			seg.segment.params.push_back(1);
			seg.segment.params.push_back(2);
			return doc;
		}
	};
}