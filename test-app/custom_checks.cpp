#include "pch.h"

void check_segment_index(const curve& curve, double x, size_t index) {
	Assert::AreEqual(index, curve.find_segment_index(x));
}
