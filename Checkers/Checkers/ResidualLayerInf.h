#pragma once
#include "ILayer.h"


class ResidualLayerInf : public ILayer<Matrix<64, 1>, Matrix<64, 1>> {

	Matrix<64, 64> m0,  m1; //Weights
	Matrix<64, 1> b0,  b1; //Biases

public:

	// Inherited via ILayer
	virtual Matrix<64, 1> forward(Matrix<64, 1> in) override;

	virtual Matrix<64, 1> backwards(Matrix<64, 1> in) override;

	virtual void finishBatch(float alpha) override;

	ResidualLayerInf();
	~ResidualLayerInf();


	// Inherited via ILayer
	virtual void save(std::ofstream& stream) const override;

	virtual void load(std::ifstream& stream) override;


	// Inherited via ILayer
	virtual void makeRandom(float range) override;

};