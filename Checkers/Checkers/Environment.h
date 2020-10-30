#pragma once
#include "PlayEnvironment.h"
#include "LearnEnvironment.h"
#include "TestEnvironment.h"

extern PlayEnvironment playEnv;
extern LearnEnvironment learnEnv;
extern TestEnvironment testEnv;

class Environment
{
	int modelId = 1;
	bool started = false;

public:

	void start();
	void save() const;
	void load();
};

