#pragma once
#include "ILayer.h"

class ResidualLayer : public ILayer<Matrix<64, 1>, Matrix<64, 1>> {

	Matrix<64, 64>* m0, * m1; //Weights
	Matrix<64, 1>* b0, * b1; //Biases

	Matrix<64, 64>* dm0, * dm1; //for updates
	Matrix<64, 1>* db0, * db1; //for updates

	Matrix<64, 1>* a0, * a1; //Activations
	Matrix<64, 1>* z0, * z1; //preActivations

public:

	// Inherited via ILayer
	virtual Matrix<64, 1> forward(Matrix<64, 1> in) override;
	virtual Matrix<64, 1> backwards(Matrix<64, 1> in) override;

	virtual void startBatch() override;
	virtual void finishBatch(float alpha) override;

	ResidualLayer();
	~ResidualLayer();


	// Inherited via ILayer
	virtual void save(std::ofstream& stream) const override;

	virtual void load(std::ifstream& stream) override;


	// Inherited via ILayer
	virtual void makeRandom(float range) override;

};