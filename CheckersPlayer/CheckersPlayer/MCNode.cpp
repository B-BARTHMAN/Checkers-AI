#include "MCNode.h"
#include <cmath>

void MCNode::generateMoves() {

	MoveGen generator = MoveGen(position);

	if (generator.moves.size() == 0) {
		isTerminalNode = true;
		return;
	}

	isLeaf = false;

	actions.resize(generator.moves.size());

	for (int i = 0; i < generator.moves.size(); ++i) {
		actions[i].move = generator.moves[i];
	}
}

int MCNode::maximizingAction(bool minimizer) const {

	float maxVal = -999999.;
	int index = 0;

	int i = 0;
	for (const ActionMove& action : actions) {

		float childVal;
		if (action.nextState == nullptr)
			childVal = 9999;
		else {
			if(minimizer)
				childVal = -action.nextState->q + 4 * sqrt(log(n) / action.nextState->n);
			else
				childVal = action.nextState->q + 4 * sqrt(log(n) / action.nextState->n);
		}

		if (childVal > maxVal) {
			index = i;
			maxVal = childVal;
		}
		++i;
	}

	return index;
}

void MCNode::update(float val)
{
	n++;
	w += val;
	q = w / n;
}

void MCNode::reset()
{
	actions.clear();
	actions.reserve(3);
	n = 0;
	w = 0;
	q = 0;
	isLeaf = true;
	isTerminalNode = false;
}
