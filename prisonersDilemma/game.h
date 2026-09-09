#pragma once

#include "gameTypes.h"
#include "players/player.h"

// Simulates whole game
class Game
{
public:
    Game(Player *playerOne, Player *playerTwo, const int rounds);

    void playGame();

    const Player *getPlayerOne() { return playerOne; }
    const Player *getPlayerTwo() { return playerTwo; }
    const std::vector<GameRound> &getGameHistory() { return gameHistory; }

    void gameHistoryDebugPrint() const;

private:
    void playRound();

    Round getRoundPerspective(const GameRound &round, bool isPlayerOne);
    void calcRoundPayoffs(GameRound &round);

    void reset();

private:
    Player *playerOne;
    Player *playerTwo;

    int totalRounds;

    // Base game history
    std::vector<GameRound> gameHistory;

    // Game history oriented for each player's perspective
    std::vector<Round> playerOneHistory;
    std::vector<Round> playerTwoHistory;
};