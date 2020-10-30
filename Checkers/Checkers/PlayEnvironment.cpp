#include "PlayEnvironment.h"
#include <thread>
#include <string>

NeuralNetInf playEnvNet;

float netEval(const Position& pos) {
	return playEnvNet.forward(pos.toVector(pos.toMove));
}
float finalEvaluator(const Position& pos) {
	int tot = _mm_popcnt_u32(pos.mans(WHITE)) - _mm_popcnt_u32(pos.mans(BLACK));
	tot += 3 * (_mm_popcnt_u32(pos.kings(WHITE)) - _mm_popcnt_u32(pos.kings(BLACK)));

	float val = tot > 0 ? 1.f : tot < 0 ? -1.f : 0;

	return val;
}

void PlayEnvironment::Player::play(float(*finalEvaluator)(const Position&), PositionStack& stack)
{
	std::cout << "Thread " << threadID << " started playing!\n";

	playing.store(true);
	for (int g = 0; g < gameCount; ++g) {

		//Reset the Gamestack
		game.reset();

		//Start from initial position
		Position pos;
		//play through a game
		int ply;
		for (ply = 0; ply < MAX_PLY; ++ply) {

			//Pause if playing is false
			if (!playing.load(std::memory_order_relaxed)) {
				goto exit;
			}

			//The Game finished
			if (MoveGen(pos).moves.size() == 0)
				break;

			//Calculate a move
			MCSearchResult result = MCSearch(pos, netEval, threadID);

			Move move;
			if (ply >= 20)
				move = result.chooseUniform(0.1);
			else {
				float temp = 3.38f * pow(.84f, ply + 1); temp = fmin(fmax(temp, 0.1), 3);
				move = result.chooseUniform(temp);
			}

			//Add position to gamestack!
			game.append(pos, result.outcome);

			//Play the Move!
			pos.makeMove(move);
		}

		//The Game Finished, evaluate the final outcome for White and update the targetvalues
		float trueOutcome = finalEvaluator(pos);//This is for white!!!

		for (int i = 0; i < ply; ++i) {
			game.setTrueOutcome(i, trueOutcome);
			trueOutcome = -trueOutcome; //For next player!
		}

		//Add GameStack to global Position Stack
		stack.append(game);
	}

	exit:
		playing = false;
		std::cout << "Thread " << threadID << " finished!\n";
}

void PlayEnvironment::Player::init(int id, int count)
{
	std::cout << "Thread " << id << " initialized with " << count << " Games!\n";
	threadID = id;
	gameCount = count;
}
PlayEnvironment::Player::Player()
{
}

PlayEnvironment::PlayEnvironment()
{
	running = true;
	playing = false;

	players = new Player[THREAD_COUNT];

	//Launch threads
	for (int threadID = 0; threadID < THREAD_COUNT; ++threadID) {

		std::thread thread([this, threadID](){

			Player* myPlayer = &players[threadID];

			while (running) {

				std::unique_lock<std::mutex> lk(sleepMutex);
				sleepCondition.wait(lk, [this]() {return this->playing.load(std::memory_order::memory_order_acquire) || !running; });//Wait here!
				lk.unlock();

				if (!running)
					break;

				myPlayer->play(finalEvaluator, positionStack);

				//Wait here after finished
				lk.lock();
				sleepCondition.wait(lk, [this]() {return !this->playing.load(std::memory_order_acquire) || !running; });
				lk.unlock();
			}

			});

		thread.detach();
	}
}

PlayEnvironment::~PlayEnvironment()
{
	running = false;
	sleepCondition.notify_all();
	delete[] players;
}

void PlayEnvironment::load(int id, bool started = false)
{
	modelID = id;

	//Load Neural Net
	std::string modelPath(MODEL_PATH);
	modelPath.append(std::to_string(modelID));

	if (modelID == 1 && started == false) {
		//First model, create it!
		playEnvNet.makeRandom();
		playEnvNet.save(modelPath.c_str());
	}
	else
		playEnvNet.load(modelPath.c_str());

	//Reset the positionstack
	positionStack.reset();

	if (started) {
		//Load Games
		std::string playPath(PLAY_PATH);
		playPath.append(std::to_string(modelID));
		positionStack.load(playPath.c_str());
	}

	int gameCount = GAME_COUNT - positionStack.count() / MAX_PLY;
	std::cout << gameCount << " games to play!\n";
	int threadGames = gameCount / THREAD_COUNT;
	int lastThreadGames = gameCount - (THREAD_COUNT - 1) * threadGames;

	for (int threadID = 0; threadID < THREAD_COUNT; ++threadID) {
		players[threadID].init(threadID, threadID == (THREAD_COUNT - 1) ? lastThreadGames : threadGames);
	}
}

void PlayEnvironment::play()
{
	std::cout << "Starting PLaying\n";
	for (int i = 0; i < THREAD_COUNT; ++i) {
		players[i].playing = true;
	}

	playing = true;
	sleepCondition.notify_all();
}

bool PlayEnvironment::isFinished() const
{
	for (int i = 0; i < THREAD_COUNT; ++i) {
		if (players[i].playing.load(std::memory_order_relaxed)) {
			return false;
		}
	}
	return true;
}
void PlayEnvironment::pause()
{
	playing = false;
	for (int i = 0; i < THREAD_COUNT; ++i) {
		players[i].playing = false;
	}
}
void PlayEnvironment::save()
{
	playing = false;
	sleepCondition.notify_all();
	std::string basePath(PLAY_PATH);
	basePath.append(std::to_string(modelID));
	positionStack.save(basePath.c_str());
}
