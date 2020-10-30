#include "InputLayerInf.h"
#include "Activation.h"

Matrix<64, 1> InputLayerInf::forward(Matrix<128, 1> in)
{
	Matrix<64, 1> ret = (m0 * in) + b0; ret.apply(ReLU);
	return ret;
}
Matrix<128, 1> InputLayerInf::backwards(Matrix<64, 1> in)
{
	return Matrix<128, 1>();
}

void InputLayerInf::finishBatch(float alpha)
{
}
InputLayerInf::InputLayerInf()
{
}
InputLayerInf::~InputLayerInf()
{
}

void InputLayerInf::save(std::ofstream& stream) const
{
	//Write weights first
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 128; ++j) {
			stream.write(reinterpret_cast<const char*>(&(m0.val[i][j])), sizeof(float));
		}
	}

	//Write biases
	for (int i = 0; i < 64; ++i) {
		stream.write(reinterpret_cast<const char*>(&(b0.val[i][0])), sizeof(float));
	}
}

void InputLayerInf::load(std::ifstream& stream)
{
	//Read weights first
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 128; ++j) {
			stream.read((char*)&m0.val[i][j], sizeof(float));
		}
	}

	//Read biases
	for (int i = 0; i < 64; ++i) {
		stream.read((char*)&b0.val[i][0], sizeof(float));
	}
}

void InputLayerInf::makeRandom(float range)
{
	m0 = Matrix<64, 128>::random() * range;
	b0 = Matrix<64, 1>::random() * range;
}
