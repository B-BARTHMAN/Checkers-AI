#pragma once
#include "types.h"
#include "Position.h"

#include <vector>

class MoveGen
{
    const Position& position;

public:
    std::vector<Move> moves;
    MoveGen(const Position& pos);
    MoveGen(const Position& pos, Square lastMoved);
};

