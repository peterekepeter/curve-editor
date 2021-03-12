#include "segment.h"

float segment::eval() const
{
	return this->eval(.5f);
}

float segment::eval(float normalized_time) const {
	return this->algorithm(normalized_time, this->params.data(), this->params.size());
}