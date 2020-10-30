#include "ValueLayerInf.h"
#include "Activation.h"

Matrix<1, 1> ValueLayerInf::forward(Matrix<64, 1> in)
{
	Matrix<1, 1> ret = (m0 * in) + b0; ret.apply(TanHAlt);
	return ret;
}

Matrix<64, 1> ValueLayerInf::backwards(Matrix<1, 1> in)
{
	return Matrix<64, 1>();
}

void ValueLayerInf::finishBatch(float alpha)
{
}

ValueLayerInf::ValueLayerInf()
{
}

ValueLayerInf::~ValueLayerInf()
{
}

void ValueLayerInf::save(std::ofstream& stream) const
{
	//Write weights first
	for (int i = 0; i < 64; ++i) {
		stream.write(reinterpret_cast<const char*>(&(m0.val[0][i])), sizeof(float));
	}

	//Write biases
	stream.write(reinterpret_cast<const char*>(&(b0.val[0][0])), sizeof(float));

}

void ValueLayerInf::load(std::ifstream& stream)
{
	//Read weights first
	for (int i = 0; i < 64; ++i) {
		stream.read((char*)&m0.val[0][i], sizeof(float));
	}

	//Read biases
	stream.read((char*)&b0.val[0][0], sizeof(float));
}

void ValueLayerInf::makeRandom(float range)
{
	m0 = Matrix<1, 64>::random() * range;
	b0 = Matrix<1, 1>::random() * range;
}
