#pragma once
#include "MCNode.h"
#include "HyperParams.h"

struct MCMove {
	Move move;
	int count;
	float outcome;

	MCMove(Move m, int c, float o)
		:move(m), count(c), outcome(o)
	{}
};

struct MCSearchResult {
	float outcome;
	std::vector<MCMove> actions;

	Move maxOutcome() const;
	Move mostPlayed() const;
	Move chooseUniform(float temp) const;
};

MCSearchResult MCSearch(const Position& pos, float (*evaluate)(const Position&), int threadID);
//network specific
MCSearchResult MCSearchSpec(const Position& pos, float (*evaluate)(const Position&, int), int parameter, int threadID);