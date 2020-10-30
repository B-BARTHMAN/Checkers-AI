#pragma once
#include "INetwork.h"
#include "InputLayerInf.h"
#include "ValueLayerInf.h"

class SimpleNetInf : public INetworkInf
{

	InputLayerInf input;
	ValueLayerInf value;

public:

	// Inherited via INetworkInf
	virtual float forward(Matrix<128, 1> in) override;
	virtual void save(const char* path) const override;
	virtual void load(const char* path) override;

	// Inherited via INetworkInf
	virtual void makeRandom() override;
};

