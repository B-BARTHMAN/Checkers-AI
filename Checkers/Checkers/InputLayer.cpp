#include "InputLayer.h"
#include "Activation.h"

Matrix<64, 1> InputLayer::forward(Matrix<128, 1> in)
{
	*a0 = in;
	Matrix<64, 1> ret = *z0 = ((*m0) * in) + *b0; ret.apply(ReLU);
	return ret;
}

Matrix<128, 1> InputLayer::backwards(Matrix<64, 1> in)
{
	auto q = *z0;
	q.apply(ReLU, true);
	q = in.hadamard(q);

	*db0 += q;//////////////////////
	*dm0 += q * a0->transpose();////

	++batchSize;

	return Matrix<128, 1>();
}

void InputLayer::startBatch()
{
	ILayer<Matrix<128, 1>, Matrix<64, 1>>::startBatch();

	dm0->clear();
	db0->clear();
}

void InputLayer::finishBatch(float alpha)
{
	float factor = alpha / batchSize;

	*m0 -= (*dm0) * factor;
	*b0 -= (*db0) * factor;
}

InputLayer::InputLayer()
{
	m0 = new Matrix<64, 128>();
	b0 = new Matrix<64, 1>();

	dm0 = new Matrix<64, 128>();
	db0 = new Matrix<64, 1>();

	a0 = new Matrix<128, 1>();
	z0 = new Matrix<64, 1>();
}

InputLayer::~InputLayer()
{
	delete m0;
	delete b0;

	delete dm0;
	delete db0;

	delete a0;
	delete z0;
}

void InputLayer::save(std::ofstream& stream) const
{
	//Write weights first
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 128; ++j) {
			stream.write(reinterpret_cast<const char*>(&(m0->val[i][j])), sizeof(float));
		}
	}

	//Write biases
	for (int i = 0; i < 64; ++i) {
		stream.write(reinterpret_cast<const char*>(&(b0->val[i][0])), sizeof(float));
	}
}

void InputLayer::load(std::ifstream& stream)
{
	//Read weights first
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 128; ++j) {	
			stream.read((char*)&m0->val[i][j], sizeof(float));
		}
	}

	//Read biases
	for (int i = 0; i < 64; ++i) {
		stream.read((char*)&b0->val[i][0], sizeof(float));
	}
}

void InputLayer::makeRandom(float range)
{
	*m0 = Matrix<64, 128>::random() * range;
	*b0 = Matrix<64, 1>::random() * range;
}
