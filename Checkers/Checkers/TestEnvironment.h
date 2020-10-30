#pragma once
#include "NeuralNetInf.h"
#include "Position.h"

#include <mutex>
#include <condition_variable>
#include <atomic>

extern NeuralNetInf newNet;
extern NeuralNetInf oldNet;

class TestEnvironment
{
	class Player {

		int threadId;

		int gameCount;

	public:

		int newWins;
		int oldWins;

		std::atomic_bool playing = false;

		void play(float(*finalEvaluator)(const Position&));
		void init(int gC, int id);
	};

	int oldId;
	int newId;

	Player* players;

	std::mutex sleepMutex;
	std::condition_variable sleepCondition;
	std::atomic_bool running;
	std::atomic_bool playing;

public:

	TestEnvironment();
	~TestEnvironment();

	void load(int oldId, int newId);
	void play();
	void wait();
};

