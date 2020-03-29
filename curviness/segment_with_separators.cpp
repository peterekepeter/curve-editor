#include "segment_with_separators.h"

float segment_with_separators::eval(int time)
{
	if (this->left >= this->right) {
		return this->segment.eval();
	}
	return this->segment.eval(this->left, time, this->right);
}
