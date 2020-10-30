#pragma once
#include "INetwork.h"

#include "InputLayer.h"
#include "ResidualLayer.h"
#include "ValueLayer.h"

class NeuralNet :
	public INetwork
{
	InputLayer input;
	ResidualLayer residual;
	ValueLayer value;

public:
	// Inherited via INetwork
	virtual float forward(Matrix<128, 1> in) override;
	virtual void backward(float delta) override;
	virtual void save(const char* path) const override;
	virtual void load(const char* path) override;
	virtual void startBatch() override;
	virtual void finishBatch(float alpha) override;
};

