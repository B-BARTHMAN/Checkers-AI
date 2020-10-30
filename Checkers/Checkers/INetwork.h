#pragma once
#include "Matrix.h"

class INetworkInf
{
public:
	virtual float forward(Matrix<128, 1> in) = 0;

	virtual void save(const char* path) const = 0;
	virtual void load(const char* path) = 0;
	virtual void makeRandom() = 0;
};

class INetwork {

public:

	virtual float forward(Matrix<128, 1> in) = 0;
	virtual void backward(float delta) = 0;

	virtual void save(const char* path) const = 0;
	virtual void load(const char* path) = 0;

	virtual void startBatch() = 0;
	virtual void finishBatch(float alpha) = 0;
};

