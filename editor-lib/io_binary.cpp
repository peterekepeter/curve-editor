#include "io_binary.h"

namespace io_binary
{
	const size_t VERSION = 1;
	const size_t MAGIC = 0x282F31BA;

	void write(ostream& out, const document_model& data)
	{
		write(out, MAGIC);
		write(out, VERSION);
		write(out, data.curve_list);
	}

	void read(istream& in, document_model& data)
	{
		size_t magic, version;
		read(in, magic);
		read(in, version);
		if (magic != MAGIC) {
			throw "Invalid format.";
		}
		if (version != VERSION) {
			throw "Unsupported version.";
		}
		read(in, data.curve_list);
	}

	void write(ostream& out, const vector<curve>& data)
	{
		write(out, data.size());
		for (auto& item : data) {
			write(out, item);
		}
	}

	void read(istream& in, vector<curve>& data)
	{
		size_t size;
		read(in, size);
		data.resize(size);
		for (auto& item : data) {
			read(in, item);
		}
	}

	void write(ostream& out, const curve& data)
	{
		{
			size_t count = data.get_separator_count();
			write(out, count);
			for (size_t i = 0; i < count; i++) {
				double value = data.get_separator_value(i);
				write(out, value);
			}
		}
		{
			size_t segment_count = data.get_segment_count();
			write(out,segment_count);
			for (size_t i = 0; i < segment_count; i++){
				auto value = data.get_segment_by_index(i);
				write(out, value.segment);
			}
		}
	}

	void read(istream& in, curve& data)
	{
		data = curve(); // init
		{
			size_t separator_count;
			read(in, separator_count);
			for (size_t i = 0; i < separator_count; i++) {
				double value;
				read(in, value);
				data.split(value);
			}
		}
		{
			size_t segment_count;
			read(in, segment_count);
			if (segment_count != data.get_segment_count()) {
				throw "data corruption";
			}
			for (size_t i = 0; i < segment_count; i++) {
				auto i_th = data.get_segment_by_index(i);
				read(in, i_th.segment);
			}
		}
	}

	static int algorithm_to_id(segment::algorithm_t algo)
	{
		if (algo == algorithms::generalized_bezier) {
			return 1;
		}
		throw "algorithm not supported";
	}

	static segment::algorithm_t id_to_algorithm(int id)
	{
		if (id == 1) {
			return algorithms::generalized_bezier;
		}
		throw "algorithm not supported";
	}

	void write(ostream& out, const segment& data)
	{
		write(out, algorithm_to_id(data.algorithm));
		write(out, data.params);
	}

	void read(istream& in, segment& data)
	{
		int id; read(in, id);
		data.algorithm = id_to_algorithm(id);
		read(in, data.params);
	}

	void write(ostream& out, const vector<float>& data)
	{
		write(out, data.size());
		for (auto& value : data) {
			write(out, value);
		}
	}

	void read(istream& in, vector<float>& data)
	{
		size_t size; 
		read(in, size);
		data.resize(size);
		for (auto& value : data) {
			read(in, value);
		}
	}

	void write(ostream& out, const size_t& data)
	{
		out.write(reinterpret_cast<const char*>(&data), sizeof(size_t));
	}

	void read(istream& in, size_t& data)
	{
		in.read(reinterpret_cast<char*>(&data), sizeof(size_t));
	}

	void write(ostream& out, const float& data)
	{
		out.write(reinterpret_cast<const char*>(&data), sizeof(float));
	}

	void read(istream& in, float& data)
	{
		in.read(reinterpret_cast<char*>(&data), sizeof(float));
	}

	void write(ostream& out, const int& data)
	{
		out.write(reinterpret_cast<const char*>(&data), sizeof(int));
	}

	void read(istream& in, int& data)
	{
		in.read(reinterpret_cast<char*>(&data), sizeof(int));
	}

	void write(ostream& out, const double& data)
	{
		out.write(reinterpret_cast<const char*>(&data), sizeof(double));
	}

	void read(istream& in, double& data)
	{
		in.read(reinterpret_cast<char*>(&data), sizeof(double));
	}
}