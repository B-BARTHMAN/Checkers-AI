#pragma once
#include "ILayer.h"

class InputLayer : public ILayer<Matrix<128, 1>, Matrix<64, 1>> {

	Matrix<64, 128>* m0; //Weights
	Matrix<64, 1>* b0; //Biases

	Matrix<64, 128>* dm0; //Weight Updates
	Matrix<64, 1>* db0; //Bias Updates

	Matrix<128, 1>* a0; //Activations
	Matrix<64, 1>* z0; //Pre Activations

public:

	// Inherited via ILayer
	virtual Matrix<64, 1> forward(Matrix<128, 1> in) override;
	virtual Matrix<128, 1> backwards(Matrix<64, 1> in) override;

	virtual void startBatch() override;
	virtual void finishBatch(float alpha) override;


	InputLayer();
	~InputLayer();


	// Inherited via ILayer
	virtual void save(std::ofstream& stream) const override;

	virtual void load(std::ifstream& stream) override;


	// Inherited via ILayer
	virtual void makeRandom(float range) override;

};