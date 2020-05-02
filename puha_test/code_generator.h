#pragma once
#include <string>
#include "../curviness/curve.h"

namespace code_generator
{
	std::string generate_c_like(const curve& curve);
}