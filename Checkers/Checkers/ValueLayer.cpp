#include "ValueLayer.h"
#include "Activation.h"

Matrix<1, 1> ValueLayer::forward(Matrix<64, 1> in)
{
	*a0 = in;
	Matrix<1, 1> ret = *z0 = ((*m0) * in) + *b0; ret.apply(TanHAlt);
	return ret;
}

Matrix<64, 1> ValueLayer::backwards(Matrix<1, 1> in)
{
	auto q = *z0;
	q.apply(TanHAlt, true);
	q = in.hadamard(q);

	*db0 += q;//////////////////////
	*dm0 += q * a0->transpose();////

	//a new entry of the batch was processed
	++batchSize;

	return m0->transpose() * q;

}

void ValueLayer::startBatch()
{
	ILayer<Matrix<64, 1>, Matrix<1, 1>>::startBatch();
	dm0->clear();
	db0->clear();
}

void ValueLayer::finishBatch(float alpha)
{
	float factor = alpha / batchSize;

	*m0 -= (*dm0) * factor;
	*b0 -= (*db0) * factor;
}

ValueLayer::ValueLayer()
{
	m0 = new Matrix<1, 64>();
	b0 = new Matrix<1, 1>(); 

	dm0 = new Matrix<1, 64>();
	db0 = new Matrix<1, 1>();

	a0 = new Matrix<64, 1>();
	z0 = new Matrix<1, 1>();
}

ValueLayer::~ValueLayer()
{
	delete m0;
	delete b0;

	delete dm0;
	delete db0;

	delete a0;
	delete z0;
}

void ValueLayer::save(std::ofstream& stream) const
{
	//Write weights first
	for (int i = 0; i < 64; ++i) {
		stream.write(reinterpret_cast<const char*>(&(m0->val[0][i])), sizeof(float));
	}

	//Write biases
	stream.write(reinterpret_cast<const char*>(&(b0->val[0][0])), sizeof(float));

}

void ValueLayer::load(std::ifstream& stream)
{
	//Read weights first
	for (int i = 0; i < 64; ++i) {
		stream.read((char*)&m0->val[0][i], sizeof(float));
	}

	//Read biases
	stream.read((char*)&b0->val[0][0], sizeof(float));
}

void ValueLayer::makeRandom(float range)
{
	*m0 = Matrix<1, 64>::random() * range;
	*b0 = Matrix<1, 1>::random() * range;
}
