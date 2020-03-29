#pragma once
#include "segment.h"

struct segment_with_separators
{
	int left, right;
	segment& segment;
	float eval(int time);
};