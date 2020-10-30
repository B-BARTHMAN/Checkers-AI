#pragma once
#include "INetwork.h"

#include "InputLayerInf.h"
#include "ResidualLayerInf.h"
#include "ValueLayerInf.h"

class NeuralNetInf :
	public INetworkInf
{
	InputLayerInf input;
	ResidualLayerInf residual;
	ValueLayerInf value;

public:

	// Inherited via INetworkInf
	virtual float forward(Matrix<128, 1> in) override;
	virtual void save(const char* path) const override;
	virtual void load(const char* path) override;
	virtual void makeRandom() override;
};

