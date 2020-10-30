#pragma once
#include "ILayer.h"

class InputLayerInf : public ILayer<Matrix<128, 1>, Matrix<64, 1>> {

	Matrix<64, 128> m0; //Weights
	Matrix<64, 1> b0; //Biases

public:

	// Inherited via ILayer
	virtual Matrix<64, 1> forward(Matrix<128, 1> in) override;
	virtual Matrix<128, 1> backwards(Matrix<64, 1> in) override;

	virtual void finishBatch(float alpha) override;


	InputLayerInf();
	~InputLayerInf();


	// Inherited via ILayer
	virtual void save(std::ofstream& stream) const override;

	virtual void load(std::ifstream& stream) override;


	// Inherited via ILayer
	virtual void makeRandom(float range) override;

};