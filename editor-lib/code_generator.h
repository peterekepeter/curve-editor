#pragma once
#include <string>
#include "../curve-lib/curve.h"
#include "../curve-lib/document_model.h"

namespace code_generator
{
	std::string generate_c_like(const document_model& curve);
}