#include "NeuralNet.h"

float NeuralNet::forward(Matrix<128, 1> in)
{
	auto k = input.forward(in);
	k = residual.forward(k);
	return value.forward(k).val[0][0];
}

void NeuralNet::backward(float delta)
{
	Matrix<1, 1> d; d.val[0][0] = delta;
	auto k = value.backwards(d);
	residual.backwards(k);
	input.backwards(k);
}

void NeuralNet::save(const char* path) const
{
	std::ofstream out;
	out.open(path, std::ios::out | std::ios::binary);

	input.save(out);
	residual.save(out);
	value.save(out);

	out.close();
}

void NeuralNet::load(const char* path)
{
	std::ifstream in;
	in.open(path, std::ios::in | std::ios::binary);

	input.load(in);
	residual.load(in);
	value.load(in);

	in.close();
}

void NeuralNet::startBatch()
{
	input.startBatch();
	residual.startBatch();
	value.startBatch();
}

void NeuralNet::finishBatch(float alpha)
{
	input.finishBatch(alpha);
	residual.finishBatch(alpha);
	value.finishBatch(alpha);
}
