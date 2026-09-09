#pragma once

#include "../player.h"

#include <iostream>
using namespace std;
class Spitefull : public Player
{
public:
    Spitefull() : Player("Spitefull")
    {
    }

    Move chooseMove(const std::vector<Round> &gameHistory) override
    {
        // If opponent defects once always defect otherwise cooperate
        if (gameHistory.empty())
        {
            return Move::Cooperate;
        }

        if (gameHistory.back().opponentMove == Move::Defect)
        {
            enemyDefected = true;
        }

        if (enemyDefected)
        {
            return Move::Defect;
        }

        return Move::Cooperate;
    }
    void reset() override
    {
        Player::score = 0;
        enemyDefected = false;
    }

private:
    bool enemyDefected = false;
};