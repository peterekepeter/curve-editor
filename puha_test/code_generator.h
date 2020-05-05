#pragma once
#include <string>
#include "../curviness/curve.h"
#include "../editor-lib/document_model.h"

namespace code_generator
{
	std::string generate_c_like(const document_model& curve);
}