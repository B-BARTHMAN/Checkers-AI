#pragma once
#include "GameStack.h"
#include "HyperParams.h"

#include <atomic>

class PositionStack
{
	GameStack::GameStackPosition* positions;
	std::atomic_size_t current;

	inline void append(const GameStack::GameStackPosition& pos) {
		positions[current++] = pos;
	}

public:
	PositionStack() {
		positions = new GameStack::GameStackPosition[MAX_PLY * GAME_COUNT];
	}
	~PositionStack() {
		delete[] positions;
	}

	void save(const char* file) const;
	void load(const char* file);

	inline const GameStack::GameStackPosition& getPos(size_t id) const {
		return positions[id];
	}

	inline void append(const GameStack& stack) {
		for (int i = 0; i < stack.current; ++i) {
			append(stack.positions[i]);
		}
	}
	inline void reset() {
		current = 0;
	}

	inline unsigned int count() const {
		return current;
	}

};

