#include "ResidualLayerInf.h"
#include "Activation.h"

Matrix<64, 1> ResidualLayerInf::forward(Matrix<64, 1> in)
{
	auto a1 = (m0 * in) + b0; a1.apply(ReLU);
	auto a2 = (m1 * a1) + b1 + in; a2.apply(ReLU);
	return a2;
}

Matrix<64, 1> ResidualLayerInf::backwards(Matrix<64, 1> in)
{
	return Matrix<64, 1>();
}

void ResidualLayerInf::finishBatch(float alpha)
{
}

ResidualLayerInf::ResidualLayerInf()
{
}

ResidualLayerInf::~ResidualLayerInf()
{
}

void ResidualLayerInf::save(std::ofstream& stream) const
{
	//Write weights first
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 64; ++j) {
			stream.write(reinterpret_cast<const char*>(&(m0.val[i][j])), sizeof(float));
			stream.write(reinterpret_cast<const char*>(&(m1.val[i][j])), sizeof(float));
		}
	}

	//Write biases
	for (int i = 0; i < 64; ++i) {
		stream.write(reinterpret_cast<const char*>(&(b0.val[i][0])), sizeof(float));
		stream.write(reinterpret_cast<const char*>(&(b1.val[i][0])), sizeof(float));
	}
}

void ResidualLayerInf::load(std::ifstream& stream)
{
	//Read weights first
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 64; ++j) {
			stream.read((char*)&m0.val[i][j], sizeof(float));
			stream.read((char*)&m1.val[i][j], sizeof(float));
		}
	}

	//Read biases
	for (int i = 0; i < 64; ++i) {
		stream.read((char*)&b0.val[i][0], sizeof(float));
		stream.read((char*)&b1.val[i][0], sizeof(float));
	}
}

void ResidualLayerInf::makeRandom(float range)
{
	m0 = Matrix<64, 64>::random() * range;
	m1 = Matrix<64, 64>::random() * range;

	b0 = Matrix<64, 1>::random() * range;
	b1 = Matrix<64, 1>::random() * range;
}

