#pragma once
#include "INetwork.h"
#include "InputLayer.h"
#include "ValueLayer.h"

class SimpleNet : INetwork {

	InputLayer input;
	ValueLayer value;

public:

	// Inherited via INetwork
	virtual float forward(Matrix<128, 1> in) override;

	virtual void backward(float delta) override;

	virtual void save(const char* path) const override;

	virtual void load(const char* path) override;


	// Inherited via INetwork
	virtual void startBatch() override;

	virtual void finishBatch(float alpha) override;

};