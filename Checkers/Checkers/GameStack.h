#pragma once
#include "Position.h"
#include "HyperParams.h"

class GameStack
{

public:

	struct GameStackPosition {
		Position pos;
		float expectedOutcome;
		float trueOutcome;
	};

	int current = 0;
	GameStackPosition positions[MAX_PLY];

public:

	inline void append(const Position& p, float expectedOutcome) {
		positions[current].pos = p;
		positions[current++].expectedOutcome = expectedOutcome;
	}

	inline void setTrueOutcome(int ply, float outcome) {
		positions[ply].trueOutcome = outcome;
	}

	inline void reset() {
		current = 0;
	}
};

