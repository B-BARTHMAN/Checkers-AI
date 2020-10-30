#include "TestRun.h"
#include<fstream>

NeuralNetInf networks[20];
std::thread threads[THREAD_COUNT];
float winValues[20][20];

float evaluate(const Position& pos, int i) {
	return networks[i].forward(pos.toVector(pos.toMove));
}
float finalEvaluate(const Position& pos) {
	int tot = _mm_popcnt_u32(pos.mans(WHITE)) - _mm_popcnt_u32(pos.mans(BLACK));
	tot += 2 * (_mm_popcnt_u32(pos.kings(WHITE)) - _mm_popcnt_u32(pos.kings(BLACK)));

	float val = tot > 0 ? 1.f : tot < 0 ? 0.f : 0.5f;

	return val;
}

void TestRun()
{
	//Load networks
	for (int player = 0; player < 20; ++player) {
		std::string name(MODEL_PATH);
		name.append(std::to_string(player + 1));
		networks[player].load(name.c_str());
	}

	//Set win values to 0
	for (int player = 0; player < 20; ++player) {
		for (int opponent = 0; opponent < 20; ++opponent) {
			winValues[player][opponent] = 0;
		}
	}

	//Thread Loop
	for (int threadID = 0; threadID < THREAD_COUNT; ++threadID) {
		threads[threadID] = std::thread([threadID]() {

			//File
			std::ofstream output(std::string("TestRun/TestRunThread").append(std::to_string(threadID)), std::ios::out | std::ios::trunc);

			//Player Loop
			for (int player = 0; player < (20 / THREAD_COUNT); ++player) {
				//Opponent Loop
				for (int opponent = 0; opponent < 20; ++opponent) {

					//Game Loop
					for (int game = 0; game < 6; ++game) {

						Position pos;
						//PLY Loop
						for (int ply = 0; ply <= 128; ++ply) {

							if (MoveGen(pos).moves.size() == 0)
								break;

							if (ply & 0b1) {
								MCSearchResult result = MCSearchSpec(pos, evaluate, opponent, threadID);
								pos.makeMove(result.chooseUniform(0.2));
							}
							else {
								MCSearchResult result = MCSearchSpec(pos, evaluate, (player * THREAD_COUNT) + threadID, threadID);
								pos.makeMove(result.chooseUniform(0.2));
							}
						}

						winValues[(player * THREAD_COUNT) + threadID][opponent] += finalEvaluate(pos);
						output << "Finished Game " << game << " between Generation " << (player * THREAD_COUNT) + threadID << " and Generation " << opponent << ": " << winValues[(player * THREAD_COUNT) + threadID][opponent] << "\n";
						output.flush();
					}

					//Log
					std::cout << "Thread " << threadID << " finished playing with white as Generation " << (player * THREAD_COUNT) + threadID << " and black as Generation " << opponent << "\n";
				}
			}

			output.close();
			});
	}

	//Join threads
	for (int threadID = 0; threadID < THREAD_COUNT; ++threadID) {
		threads[threadID].join();
	}

	//Print Table
	for (int player = 0; player < 20; ++player) {
		std::cout << "White " << player << ":\n";
		for (int opponent = 0; opponent < 20; ++opponent) {
			std::cout << "    Black " << opponent << ": " << winValues[player][opponent] << "\n";
		}
	}
}
