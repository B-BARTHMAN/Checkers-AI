#include "SimpleNet.h"
#include <fstream>

float SimpleNet::forward(Matrix<128, 1> in)
{
	auto k = input.forward(in);
	return value.forward(k).val[0][0];
}

void SimpleNet::backward(float delta)
{
	Matrix<1, 1> d; d.val[0][0] = delta;

	auto k = value.backwards(d);
	input.backwards(k);
}

void SimpleNet::save(const char* path) const
{
	std::ofstream out;
	out.open(path, std::ios::out | std::ios::binary);

	input.save(out);
	value.save(out);

	out.close();
}

void SimpleNet::load(const char* path)
{
	std::ifstream in;
	in.open(path, std::ios::in | std::ios::binary);

	input.load(in);
	value.load(in);

	in.close();
}

void SimpleNet::startBatch()
{
	input.startBatch();
	value.startBatch();
}

void SimpleNet::finishBatch(float alpha)
{
	input.finishBatch(alpha);
	value.finishBatch(alpha);
}