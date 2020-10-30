#include "Environment.h"
#include <Windows.h>
#define _CRT_SECURE_NO_WARNINGS_
#include<ctime>

PlayEnvironment playEnv;
LearnEnvironment learnEnv;
TestEnvironment testEnv;

void Environment::start()
{
	auto lastPause = std::chrono::high_resolution_clock::now();
	char time[26];

	while (true) {

		//Start playing
		auto givemetime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		ctime_s(time, sizeof(time), &givemetime);
		std::cout << time << ":\n";

		std::cout << "Playing Games for Generation " << modelId << "\n";
		std::cout << "Press [ESC] to exit\n";

		playEnv.load(modelId, started);
		playEnv.play();

		while (true) {

			if (playEnv.isFinished())
				break;

			if (GetKeyState(VK_ESCAPE) & 0x8000)
			{
				playEnv.pause();
				playEnv.save();
				playEnv.running = false;
				started = true;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				save();
				return;
			}

			if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - lastPause).count() >= PAUSE_INTERVAL) {
				playEnv.pause();
				playEnv.save();

				givemetime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				ctime_s(time, sizeof(time), &givemetime);
				std::cout << time << " : ";
				std::cout << "Performing Pause! \n";

				std::this_thread::sleep_for(std::chrono::seconds(PAUSE_TIME));

				lastPause = std::chrono::high_resolution_clock::now();

				playEnv.load(modelId, true);
				playEnv.play();
			}

		}
		playEnv.save();

		//Start learning
		givemetime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		ctime_s(time, sizeof(time), &givemetime);
		std::cout << time << ":\n";

		std::cout << "Learning from Games for Generation " << modelId << "\n";

		learnEnv.load(modelId);
		learnEnv.learn();

		//Test new model
		givemetime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		ctime_s(time, sizeof(time), &givemetime);
		std::cout << time << ":\n";

		/*
		std::cout << "Comparing Generation " << (modelId + 1) << " with Generation " << modelId << "\n";
		testEnv.load(modelId, ++modelId);
		testEnv.play();
		testEnv.wait();
		*/
		++modelId;
		started = false;
	}
}

void Environment::save() const
{
	std::ofstream output("State.bin", std::ios::out | std::ios::trunc);
	output << modelId;
	output.close();
}
void Environment::load()
{
	std::ifstream output("State.bin", std::ios::in);
	output >> modelId;
	started = true;
	output.close();
}
