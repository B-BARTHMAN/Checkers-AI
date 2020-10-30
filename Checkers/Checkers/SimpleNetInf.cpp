#include "SimpleNetInf.h"

float SimpleNetInf::forward(Matrix<128, 1> in)
{
	auto k = input.forward(in);
	return value.forward(k).val[0][0];
}

void SimpleNetInf::save(const char* path) const
{
	std::ofstream out;
	out.open(path, std::ios::out | std::ios::binary);

	input.save(out);
	value.save(out);

	out.close();
}

void SimpleNetInf::load(const char* path)
{
	std::ifstream in;
	in.open(path, std::ios::in | std::ios::binary);

	input.load(in);
	value.load(in);

	in.close();
}

void SimpleNetInf::makeRandom()
{
	input.makeRandom(0.125f);
	value.makeRandom(0.177f);
}
