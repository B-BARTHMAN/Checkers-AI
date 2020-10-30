#include "LearnEnvironment.h"

#include <string>

#include "PositionStack.h"
#include "PRNG.h"
#include "HyperParams.h"

void LearnEnvironment::load(int id)
{
	modelID = id;

	std::string basePath(MODEL_PATH);
	basePath.append(std::to_string(id));

	network.load(basePath.c_str()); //Load correct network
}

void LearnEnvironment::learn()
{
	PRNG prng;

	PositionStack positions;

	int history = historySize();

	int batchPerGen = BATCH_COUNT / history;
	int lastGenBatches = BATCH_COUNT - (history - 1) * batchPerGen;

	float totalError = 0;
	int iterations = 0;

	for (int generation = modelID - history + 1; generation <= modelID; ++generation) {

		std::string basePath(PLAY_PATH);
		basePath.append(std::to_string(generation));

		positions.load(basePath.c_str());

		int batches = generation == modelID ? lastGenBatches : batchPerGen;

		for (int batch = 0; batch < batches; ++batch) {

			network.startBatch();
			for (int sample = 0; sample < BATCH_SIZE; ++sample) {

				//choose a random sample and learn from it
				size_t index = prng.rand64() % positions.count();
				const GameStack::GameStackPosition& pos = positions.getPos(index);

				float output = network.forward(pos.pos.toVector(pos.pos.toMove));
				float delta = output - 0.5f * (pos.trueOutcome + pos.expectedOutcome);
				network.backward(delta);

				totalError += 0.5 * delta * delta;
				++iterations;
			}
			network.finishBatch(LEARNINGRATE);
		}
	}


	std::cout << "Total Error: " << (totalError / iterations) << "\n";
	//after learning, save the new model
	std::string basePath(MODEL_PATH); basePath.append(std::to_string(modelID + 1));///////////////////
	network.save(basePath.c_str());////////////////////////////////////////////////
}
