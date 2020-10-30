#pragma once
#include "Matrix.h"
#include <fstream>

template<class A, class B>
class ILayer {

public:
	virtual B forward(A in) = 0;
	virtual A backwards(B in) = 0;

	virtual void save(std::ofstream& stream) const = 0;
	virtual void load(std::ifstream& stream) = 0;

	short batchSize = 0;

	virtual void makeRandom(float range) = 0;

protected:
	virtual void startBatch() {
		batchSize = 0;
	}
	virtual void finishBatch(float alpha) = 0;

};