#include "TestEnvironment.h"

#include "MCSearch.h"
#include "HyperParams.h"
#include <string>

NeuralNetInf newNet;
NeuralNetInf oldNet;

float evalNew(const Position& pos) {
	return newNet.forward(pos.toVector(pos.toMove));
}
float evalOld(const Position& pos) {
	return oldNet.forward(pos.toVector(pos.toMove));
}

float finalEval(const Position& pos) {
	int tot = _mm_popcnt_u32(pos.mans(WHITE)) - _mm_popcnt_u32(pos.mans(BLACK));
	tot += 3 * (_mm_popcnt_u32(pos.kings(WHITE)) - _mm_popcnt_u32(pos.kings(BLACK)));

	float val = tot > 0 ? 1.f : tot < 0 ? -1.f : 0;

	return val;
}

void TestEnvironment::Player::play(float(*finalEvaluator)(const Position&))
{
	newWins = 0;
	oldWins = 0;

	for (int game = 0; game < gameCount; ++game) {

		Position pos;

		for (int ply = 0; ply < MAX_PLY; ++ply) {

			if (MoveGen(pos).moves.size() == 0)
				break;

			MCSearchResult res;

			//Play with white
			if ((game & 0b1) == 0) {

				if ((ply & 0b1) == 0)
					res = MCSearch(pos, evalNew, threadId);
				else
					res = MCSearch(pos, evalOld, threadId);
			}
			//play with black
			else {

				if ((ply & 0b1) == 0)
					res = MCSearch(pos, evalOld, threadId);
				else
					res = MCSearch(pos, evalNew, threadId);
			}

			Move move;

			/*
			if (ply >= 10)
				move = res.mostPlayed();
			else {
				float temp = 1.26 * pow(.78f, ply + 1); temp = fmin(fmax(temp, 0.1), 1);
				move = res.chooseUniform(temp);
			}
			*/
			move = res.mostPlayed();
			pos.makeMove(move);

		}

		float whiteVal = finalEval(pos);

		//Draw
		if (whiteVal == 0)
			continue;

		if ((game & 0b1) == 0) {
			if (whiteVal == 1)
				++newWins;
			else
				++oldWins;
		}
		else {
			if (whiteVal == 1)
				++oldWins;
			else
				++newWins;
		}

	}

	playing = false;
}

void TestEnvironment::Player::init(int gC, int id)
{
	gameCount = gC;
	threadId = id;
}

TestEnvironment::TestEnvironment() {

	running = true;
	playing = false;
	
	players = new Player[THREAD_COUNT];

	int games = TEST_COUNT / THREAD_COUNT;
	int lastGames = TEST_COUNT - (THREAD_COUNT - 1) * games;

	for (int threadId = 0; threadId < THREAD_COUNT; ++threadId) {

		players[threadId].init(threadId == (THREAD_COUNT - 1) ? lastGames : games, threadId);
		Player* player = &players[threadId];

		std::thread thread([this, player]() {

			while (running) {

				std::unique_lock<std::mutex> lk(sleepMutex);
				sleepCondition.wait(lk, [this]() {return this->playing.load(std::memory_order::memory_order_acquire) || !running; });
				lk.unlock();

				if (!running)
					break;

				player->play(finalEval);

				//Wait here after finished
				lk.lock();
				sleepCondition.wait(lk, [this]() {return !this->playing.load(std::memory_order_acquire) || !running; });
				lk.unlock();
			}

			});

		thread.detach();
	}
}

TestEnvironment::~TestEnvironment()
{
	running = false;
	sleepCondition.notify_all();
	delete[] players;
}

void TestEnvironment::load(int oldId, int newId)
{
	std::string oldPath(MODEL_PATH);
	std::string newPath(MODEL_PATH);

	oldPath.append(std::to_string(oldId));
	newPath.append(std::to_string(newId));

	oldNet.load(oldPath.c_str());
	newNet.load(newPath.c_str());
}

void TestEnvironment::play()
{
	for (int i = 0; i < THREAD_COUNT; ++i) {
		players[i].playing = true;
	}
	playing = true;
	sleepCondition.notify_all();
}

void TestEnvironment::wait()
{
	while (true) {

		bool waiting = false;
		for (int i = 0; i < THREAD_COUNT; ++i) {
			if (players[i].playing.load(std::memory_order_relaxed)) {
				waiting = true;
				break;
			}
		}

		if (!waiting)
			break;
	}

	playing = false;
	sleepCondition.notify_all();

	int oldWins = 0;
	int newWins = 0;

	for (int i = 0; i < THREAD_COUNT; ++i) {
		oldWins += players[i].oldWins;
		newWins += players[i].newWins;
	}

	std::cout << "newWins: " << newWins << "\n";
	std::cout << "oldWins: " << oldWins << "\n";

	if (newWins > oldWins)
		std::cout << "ITS BETTER!!!!!\n";
	else
		std::cout << "ITS WORSE(or equal)!!!!!\n";
}
