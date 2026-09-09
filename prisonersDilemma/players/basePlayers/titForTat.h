#pragma once

#include "../player.h"

class TitForTat : public Player
{
public:
    TitForTat() : Player("Tit For Tat")
    {
    }

    Move chooseMove(const std::vector<Round> &gameHistory) override
    {
        // Start game by cooperating
        if (gameHistory.empty())
        {
            return Move::Cooperate;
        }

        // Copy oppenents move from previous round
        return gameHistory.back().opponentMove;
    }
};