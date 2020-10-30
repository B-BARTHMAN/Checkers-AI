#include "ResidualLayer.h"
#include "Activation.h"

Matrix<64, 1> ResidualLayer::forward(Matrix<64, 1> in) {

	*a0 = in;

	*z0 = *a1 = ((*m0) * in) + *b0; a1->apply(ReLU);

	Matrix<64, 1> ret = *z1 = ((*m1) * (*a1)) + *b1 + *a0; ret.apply(ReLU);
	return ret;
}

Matrix<64, 1> ResidualLayer::backwards(Matrix<64, 1> in) {

	auto q = *z1;
	q.apply(ReLU, true);
	q = in.hadamard(q); // dE/dci

	*db1 += q;/////////////////////////////////////
	*dm1 += q * a1->transpose();//////////////////////

	auto p = *z0; p.apply(ReLU, true);
	p = p.hadamard(m1->transpose() * q);

	*db0 += p;/////////////////////////////////////
	*dm0 += p * a0->transpose();//////////////////////

	//a new entry of the batch was processed
	++batchSize;

	return (m0->transpose() * p) + q;

}

void ResidualLayer::startBatch() {
	ILayer<Matrix<64, 1>, Matrix<64, 1>>::startBatch();

	//set dm0, dm1, db0, db1 to zero
	dm0->clear();
	dm1->clear();
	db0->clear();
	db1->clear();
}

void ResidualLayer::finishBatch(float alpha) {
	float factor = alpha / batchSize;

	*m0 -= (*dm0) * factor;
	*m1 -= (*dm1) * factor;
	*b0 -= (*db0) * factor;
	*b1 -= (*db1) * factor;
}

ResidualLayer::ResidualLayer()
{
	m0 = new Matrix<64, 64>();
	m1 = new Matrix<64, 64>();

	b0 = new Matrix<64, 1>(); 
	b1 = new Matrix<64, 1>(); 

	dm0 = new Matrix<64, 64>();
	dm1 = new Matrix<64, 64>();

	db0 = new Matrix<64, 1>();
	db1 = new Matrix<64, 1>();

	a0 = new Matrix<64, 1>();
	a1 = new Matrix<64, 1>();

	z0 = new Matrix<64, 1>();
	z1 = new Matrix<64, 1>();
}

ResidualLayer::~ResidualLayer()
{
	delete m0; delete m1;
	delete b0; delete b1;
	delete dm0; delete dm1;
	delete db0; delete db1;
	delete a0; delete a1;
	delete z0; delete z1;
}

void ResidualLayer::save(std::ofstream& stream) const
{
	//Write weights first
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 64; ++j) {
			stream.write(reinterpret_cast<const char*>(&(m0->val[i][j])), sizeof(float));
			stream.write(reinterpret_cast<const char*>(&(m1->val[i][j])), sizeof(float));
		}
	}

	//Write biases
	for (int i = 0; i < 64; ++i) {
		stream.write(reinterpret_cast<const char*>(&(b0->val[i][0])), sizeof(float));
		stream.write(reinterpret_cast<const char*>(&(b1->val[i][0])), sizeof(float));
	}
}

void ResidualLayer::load(std::ifstream& stream)
{
	//Read weights first
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 64; ++j) {
			stream.read((char*)&m0->val[i][j], sizeof(float));
			stream.read((char*)&m1->val[i][j], sizeof(float));
		}
	}

	//Read biases
	for (int i = 0; i < 64; ++i) {
		stream.read((char*)&b0->val[i][0], sizeof(float));
		stream.read((char*)&b1->val[i][0], sizeof(float));
	}
}

void ResidualLayer::makeRandom(float range)
{
	*m0 = Matrix<64, 64>::random() * range;
	*m1 = Matrix<64, 64>::random() * range;

	*b0 = Matrix<64, 1>::random() * range;
	*b1 = Matrix<64, 1>::random() * range;
}
