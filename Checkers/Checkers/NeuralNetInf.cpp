#include "NeuralNetInf.h"

float NeuralNetInf::forward(Matrix<128, 1> in)
{
	auto k = input.forward(in);
	k = residual.forward(k);
	return value.forward(k).val[0][0];
}

void NeuralNetInf::save(const char* path) const
{
	std::ofstream out;
	out.open(path, std::ios::out | std::ios::binary);

	input.save(out);
	residual.save(out);
	value.save(out);

	out.close();
}

void NeuralNetInf::load(const char* path)
{
	std::ifstream in;
	in.open(path, std::ios::in | std::ios::binary);

	input.load(in);
	residual.load(in);
	value.load(in);

	in.close();
}

void NeuralNetInf::makeRandom()
{
	input.makeRandom(0.125f);
	residual.makeRandom(0.177f);
	value.makeRandom(0.177f);
}
