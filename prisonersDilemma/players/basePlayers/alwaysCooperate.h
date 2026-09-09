#pragma once

#include "../player.h"

class AlwaysCooperate : public Player
{
public:
    AlwaysCooperate() : Player("Always Coop") {}

    // Game history ommitted as not used
    Move chooseMove(const std::vector<Round> &) override
    {
        return Move::Cooperate;
    }
};