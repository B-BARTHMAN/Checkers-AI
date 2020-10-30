#include "MCSearch.h"
#include <cmath>
#include "PRNG.h"

#define ITER_COUNT 8192*8

MCSearchResult MCSearch(const Position& pos, float (*evaluate)(const Position&)) {

	static MCNode nodes[ITER_COUNT];
	int nodeCount = 0;

	nodes[nodeCount++].reset();
	nodes[0].position = pos;

	MCNode* node;
	std::vector<MCNode*> searchpath;

	int iter = 0;

	while (true) {

		//buffer is full
		if (nodeCount == ITER_COUNT - 1 || iter == ITER_COUNT)
			break;

		node = &nodes[0]; // starting position

		//searchpath reset
		searchpath.clear();

		//terminalnodes
		bool terminal = false;

		//is the current node the minimizer?
		bool minimizer = true;

		/////Selection & Expansion/////
		while (true) {

			//add this node to the searchpath
			searchpath.emplace_back(node);

			//Switch the minimizer
			minimizer = !minimizer;

			if (node->isTerminalNode) {
				terminal = true;
				break;
			}

			//generate actions
			if (node->isLeaf) {
				node->generateMoves();
				if (node->isTerminalNode)
					terminal = true;
				break;
			}

			ActionMove& action = node->actions[node->maximizingAction(minimizer)];

			//Expansion
			if (action.nextState == nullptr) {
				action.nextState = &nodes[nodeCount]; //set the next state of the action
				nodes[nodeCount].reset();
				nodes[nodeCount].position = node->position.doMove(action.move); //set the new position
				node = &nodes[nodeCount++]; //this is the selected node
			}
			//March along the tree
			else {
				node = action.nextState;
			}
		}


		//EVALUATION
		float value;
		if (terminal)
			value = -1;
		else
			value = evaluate(node->position);

		//if the evaluated node was the minimizers, flip the sign
		if (minimizer)
			value = -value;

		//backpropagate
		for (MCNode* backNode : searchpath)
			backNode->update(value);

		++iter;

	}

	MCSearchResult res;
	res.outcome = nodes[0].q;

	for (ActionMove& move : nodes[0].actions) {
		res.actions.emplace_back(move.move, move.nextState->n, move.nextState->q);
	}

	return res;
}

Move MCSearchResult::maxOutcome() const
{
	Move m;
	float maxOutcome = -999;
	for (const MCMove& move : actions) {
		if (move.outcome > maxOutcome) {
			m = move.move;
			maxOutcome = move.outcome;
		}
	}
	return m;
}

Move MCSearchResult::mostPlayed() const
{
	Move m;
	float mostPlayed = -999;
	for (const MCMove& move : actions) {
		if (move.count > mostPlayed) {
			m = move.move;
			mostPlayed = move.count;
		}
	}
	return m;
}

Move MCSearchResult::chooseUniform(float temp) const
{
	std::vector<float> chances; chances.reserve(actions.size()); //Create the chances

	//calculate total moveCount(1024)
	int total = 0;
	for (const MCMove& move : actions) {
		total += move.count;
	}

	//calculate temperature
	temp = 1.f / temp;

	//Initialize chances
	for (const MCMove& move : actions) {
		chances.emplace_back(pow((float)move.count / total,temp));
	}

	//calculate new Total
	float newTotal = 0;
	for (const float& c : chances) {
		newTotal += c;
	}

	//final chance update
	float chanceTotal = 0;
	for (float& c : chances) {
		c = chanceTotal + (c / newTotal);
		chanceTotal = c;
	}

	//finally chose a move!
	int i = 0;
	float p = (s_PRNG.rand64() & 0b111111111111111ull) / 32767.f;

	for (const float& c : chances) {
		if (p > c) {
			++i;
			continue;
		}
		break;
	}

	return actions[i].move;
}
