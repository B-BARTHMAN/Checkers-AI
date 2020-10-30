#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "NeuralNetInf.h"
#include "MCSearch.h"

#include "GameStack.h"
#include "PositionStack.h"
#include "HyperParams.h"

extern NeuralNetInf playEnvNet;

class PlayEnvironment
{
	int modelID;

	PositionStack positionStack;

	class Player {
		GameStack game;
		int threadID;
		int gameCount;

	public:
		void play(float(*finalEvaluator)(const Position&), PositionStack& stack);
		void init(int id, int count);
		Player();

		std::atomic_bool playing = false;
	};

	Player* players;

	std::atomic_bool playing;

	std::mutex sleepMutex;

public:
	std::atomic_bool running;

private:
	std::condition_variable sleepCondition;

public:
	PlayEnvironment();

	void load(int modelID, bool started);

	~PlayEnvironment();

	void play();
	bool isFinished() const;
	void pause();
	void save();
};