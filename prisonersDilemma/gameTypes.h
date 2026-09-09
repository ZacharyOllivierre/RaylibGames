#pragma once

enum class Move
{
    Defect,
    Cooperate
};

enum class Outcome
{
    MutualCooperation,
    MutualDefection,
    PlayerOneSteal,
    PlayerTwoSteal
};

struct GameRound
{
    Move playerOneMove;
    Move playerTwoMove;

    int playerOnePayOff;
    int playerTwoPayOff;

    int playerOneScore;
    int playerTwoScore;

    Outcome outcome;
};

// Game round from a players perspective
struct Round
{
    Move myMove;
    Move opponentMove;

    int myPayoff;
    int opponentPayoff;

    int myScore;
    int opponentScore;
};