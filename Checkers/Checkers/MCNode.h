#pragma once

#include "Position.h"
#include "MoveGen.h"
#include <vector>

class MCNode;

struct ActionMove {
	MCNode* nextState = nullptr;
	Move move;
};

class MCNode {
public:

	Position position;

	std::vector<ActionMove> actions;

	bool isLeaf = true;
	bool isTerminalNode = false;

	int n = 0; // number of times this action has been chosen
	float w = 0; //total value
	float q = 0; //mean value

	MCNode() {}

	//generate moves
	void generateMoves();

	//return the Q + U maximizing action index
	int maximizingAction(bool minimizer) const;

	//update the values of this node
	void update(float val);

	//reset this node
	void reset();
};