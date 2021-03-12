#pragma once
#include <ostream>
#include <istream>
#include <vector>
#include "document_model.h";


namespace io_binary
{
	using std::ostream;
	using std::istream;
	using std::vector;

	void write(ostream& out, const document_model& data);
	void write(ostream& out, const vector<curve>& data);
	void write(ostream& out, const curve& data);
	void write(ostream& out, const segment& data);
	void write(ostream& out, const vector<float>& data);
	void write(ostream& out, const size_t& data);
	void write(ostream& out, const float& data);
	void write(ostream& out, const double& data);
	void write(ostream& out, const int& data);

	void read(istream& in, document_model& data);
	void read(istream& in, vector<curve>& data);
	void read(istream& in, curve& data);
	void read(istream& in, segment& data);
	void read(istream& in, vector<float>& data);
	void read(istream& in, size_t& data);
	void read(istream& in, float& data);
	void read(istream& in, double& data);
	void read(istream& in, int& data);
}