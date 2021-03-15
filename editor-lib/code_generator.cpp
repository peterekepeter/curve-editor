#include "code_generator.h"
#include <sstream>

static void generate_divide(
	std::stringstream& out,
	const curve& curve,
	size_t start,
	size_t end); // forward

static void generate_segment(
	std::stringstream& out,
	const curve& curve,
	size_t segment_index); // forward

static void generate_c_like_curve(
	size_t curve_index,
	std::stringstream& out,
	const curve& curve); // forward;

std::string code_generator::generate_c_like(const document_model& document)
{
	std::stringstream out;
	for (size_t i = 0; i < document.curve_list.size(); i++) {
		auto& curve = document.curve_list[i];
		generate_c_like_curve(i, out, curve);
		out << "\n";
	}
	return out.str();
}

static void generate_c_like_curve(
	size_t curve_index, 
	std::stringstream& out, 
	const curve& curve)
{
	auto segment_count = curve.get_separator_count() + 1;
	const auto out_type = "float";
	const auto in_type = "float";
	const auto fn_name = "untitled";
	out << out_type << " " << fn_name << curve_index;
	out << "(" << in_type << " x) { return ";

	generate_divide(out, curve, 0, segment_count - 1);

	out << "; }";
}

static void generate_divide(
	std::stringstream& out, 
	const curve& curve, 
	size_t start, 
	size_t end)
{
	if (start == end) {
		generate_segment(out, curve, start);
		return;
	}
	size_t mid = (start + end) / 2;
	auto separator_value = curve.get_separator_value(mid);
	out << "x<" << separator_value << "?";
	generate_divide(out, curve, start, mid);
	out << ":";
	generate_divide(out, curve, mid + 1, end);
}

static double get_polynomial_coef(
	int j, const std::vector<float>& params); // forward

void generate_segment(
	std::stringstream& out, 
	const curve& curve, size_t 
	segment_index)
{
	auto target = curve.get_segment_by_index(segment_index);
	if (target.segment.algorithm != algorithms::generalized_bezier) {
		throw "not supported";
	}
	auto& params = target.segment.params;
	auto count = params.size();
	auto left = target.left;
	auto length = target.right - target.left;
	if (count == 0) {
		out << "0";
		return;
	}
	if (count == 1) {
		out << params[0];
		return;
	}
	if (left == 0) {
		out << "b(x";
	}
	else {
		out << "b((x" << std::showpos << -left << ")";
		out << std::noshowpos;
	}
	if (length != 1) {
		out << "/" << length;
	}
	for (int i = 0; i < count; i++) {
		out << "," << params[i];
	}
	out << ")";
}

static int factorial(int n) {
	int result = 1;
	for (int i = 1; i < n; i++) {
		result *= i;
	}
	return result;
}

std::vector<double> get_polynomial_form(
	const std::vector<float>& params,
	double left = 0,
	double right = 1)
{
	std::vector<double> result(params.size());
	auto length = right - left;
	auto count = params.size();
	double coef_div = 1.0;
	for (int i = 0; i < count; i++) {
		auto coef = get_polynomial_coef(i, params);
		coef /= coef_div;
		result[i] = coef;
		coef_div *= length;
	}
	/// TODO: Shift to left
	return result;
}

static double get_polynomial_coef(int j, const std::vector<float>& params) {
	int n = (int)params.size();
	double product = 1.0;
	for (int m = 0; m < j; m++) {
		product *= (n - m);
	}
	double sum = 0.0;
	double sign = j % 2 == 0 ? 1.0 : -1.0; 
	for (int i = 0; i <= j; i++) {
		double part = params[i] * sign;
		part /= factorial(i) * factorial(j - i);
		sum += part;
		sign *= -1.0;
	}
	return sum * product;
}