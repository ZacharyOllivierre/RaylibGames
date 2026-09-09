#pragma once

#include "../player.h"

#include <cstdlib>
#include <ctime>

class Random : public Player
{
public:
    Random() : Player("Random")
    {
        srand(time(nullptr));
    }

    Move chooseMove(const std::vector<Round> &) override
    {
        int randomVal = rand() % 2;

        if (randomVal == 0)
        {
            return Move::Cooperate;
        }
        else
        {
            return Move::Defect;
        }
    }
};