#pragma once

#include "../player.h"

class AlwaysDefect : public Player
{
public:
    AlwaysDefect() : Player("Always Defect") {}

    // Game history ommitted as not used
    Move chooseMove(const std::vector<Round> &) override
    {
        return Move::Defect;
    }
};