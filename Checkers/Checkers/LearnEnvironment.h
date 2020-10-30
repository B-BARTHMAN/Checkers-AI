#pragma once
#include "NeuralNet.h"

class LearnEnvironment
{
	NeuralNet network;
	int modelID;

private:
	int historySize() const {
		if (modelID <= 4)
			return modelID;
		else if (modelID <= 37)
			return 4 + (modelID - 5) / 2;
		else
			return 20;
	}

public:

	void load(int id);

	void learn();
};

