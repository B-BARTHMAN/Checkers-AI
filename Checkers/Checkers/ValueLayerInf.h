#pragma once
#include "ILayer.h"

class ValueLayerInf : public ILayer<Matrix<64, 1>, Matrix<1, 1>> {

	Matrix<1, 64> m0; //Weights
	Matrix<1, 1> b0; //Biases

public:

	// Inherited via ILayer
	virtual Matrix<1, 1> forward(Matrix<64, 1> in) override;
	virtual Matrix<64, 1> backwards(Matrix<1, 1> in) override;

	virtual void finishBatch(float alpha) override;

	ValueLayerInf();
	~ValueLayerInf();


	// Inherited via ILayer
	virtual void save(std::ofstream& stream) const override;

	virtual void load(std::ifstream& stream) override;


	// Inherited via ILayer
	virtual void makeRandom(float range) override;

};