#pragma once
#include "ILayer.h"

class ValueLayer : public ILayer<Matrix<64, 1>, Matrix<1, 1>> {

	Matrix<1, 64>* m0; //Weights
	Matrix<1, 1>* b0; //Biases

	Matrix<1, 64>* dm0; //Weight Updates
	Matrix<1, 1>* db0; //Bias Updates

	Matrix<64, 1>* a0; //Activations
	Matrix<1, 1>* z0; //Pre Activations

public:

	// Inherited via ILayer
	virtual Matrix<1, 1> forward(Matrix<64, 1> in) override;

	virtual Matrix<64, 1> backwards(Matrix<1, 1> in) override;

	virtual void startBatch() override;

	virtual void finishBatch(float alpha) override;

	ValueLayer();
	~ValueLayer();


	// Inherited via ILayer
	virtual void save(std::ofstream& stream) const override;

	virtual void load(std::ifstream& stream) override;


	// Inherited via ILayer
	virtual void makeRandom(float range) override;

};