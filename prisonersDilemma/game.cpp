#include "game.h"

// For debug print
#include <iostream>
#include <iomanip>

Game::Game(Player *playerOne, Player *playerTwo, const int rounds)
{
    this->playerOne = playerOne;
    this->playerTwo = playerTwo;

    totalRounds = rounds;
}

// Plays all rounds of a game stores history
void Game::playGame()
{
    for (int i = 0; i < totalRounds; i++)
    {
        playRound();
    }
}

void Game::playRound()
{
    GameRound thisRound;

    // Get player moves
    thisRound.playerOneMove = playerOne->chooseMove(playerOneHistory);
    thisRound.playerTwoMove = playerTwo->chooseMove(playerTwoHistory);

    // Calculate payoff to each player
    calcRoundPayoffs(thisRound);

    // Update player score
    playerOne->addScore(thisRound.playerOnePayOff);
    playerTwo->addScore(thisRound.playerTwoPayOff);

    // Get player scores for after current round
    thisRound.playerOneScore = playerOne->getScore();
    thisRound.playerTwoScore = playerTwo->getScore();

    // Finish round | update data
    gameHistory.push_back(thisRound);
    playerOneHistory.push_back(getRoundPerspective(thisRound, true));
    playerTwoHistory.push_back(getRoundPerspective(thisRound, false));
}

Round Game::getRoundPerspective(const GameRound &gameRound, bool isPlayerOne)
{
    Round round;

    if (isPlayerOne)
    {
        round.myMove = gameRound.playerOneMove;
        round.opponentMove = gameRound.playerTwoMove;

        round.myPayoff = gameRound.playerOnePayOff;
        round.opponentPayoff = gameRound.playerTwoPayOff;

        round.myScore = gameRound.playerOneScore;
        round.opponentScore = gameRound.playerTwoScore;
    }
    else
    {
        round.myMove = gameRound.playerTwoMove;
        round.opponentMove = gameRound.playerOneMove;

        round.myPayoff = gameRound.playerTwoPayOff;
        round.opponentPayoff = gameRound.playerOnePayOff;

        round.myScore = gameRound.playerTwoScore;
        round.opponentScore = gameRound.playerOneScore;
    }

    return round;
}

void Game::calcRoundPayoffs(GameRound &round)
{
    // Declare points by outcome
    const int successfulCoop = 3;
    const int unsuccessfulCoop = 0;
    const int successfulSteal = 5;
    const int unsuccessfulSteal = 1;

    // Both cooperate
    if (round.playerOneMove == Move::Cooperate &&
        round.playerTwoMove == Move::Cooperate)
    {
        round.playerOnePayOff = successfulCoop;
        round.playerTwoPayOff = successfulCoop;

        round.outcome = Outcome::MutualCooperation;
    }

    // Both steal
    else if (round.playerOneMove == Move::Defect &&
             round.playerTwoMove == Move::Defect)
    {
        round.playerOnePayOff = unsuccessfulSteal;
        round.playerTwoPayOff = unsuccessfulSteal;

        round.outcome = Outcome::MutualDefection;
    }

    // Player one steal
    else if (round.playerOneMove == Move::Defect &&
             round.playerTwoMove == Move::Cooperate)
    {
        round.playerOnePayOff = successfulSteal;
        round.playerTwoPayOff = unsuccessfulCoop;

        round.outcome = Outcome::PlayerOneSteal;
    }

    // Player two steal
    else if (round.playerOneMove == Move::Cooperate &&
             round.playerTwoMove == Move::Defect)
    {
        round.playerOnePayOff = unsuccessfulCoop;
        round.playerTwoPayOff = successfulSteal;

        round.outcome = Outcome::PlayerTwoSteal;
    }
}

void Game::reset()
{
    // Reset histories
    gameHistory.clear();
    playerOneHistory.clear();
    playerTwoHistory.clear();

    // Reset players
    playerOne->reset();
    playerTwo->reset();
}

// Warning Gross ass function
void Game::gameHistoryDebugPrint() const
{
    std::string playerOneName = playerOne->getName();
    std::string playerTwoName = playerTwo->getName();
    std::string heavyHalfSeperator = "=======================================";

    std::cout << "\n"
              << heavyHalfSeperator << "\033[1m" << " GAME HISTORY " << "\033[0m"
              << heavyHalfSeperator << "\n\n";

    std::cout << "\033[1m" << std::left
              << std::setw(8) << "Round"
              << std::setw(20) << playerOneName
              << std::setw(20) << playerTwoName
              << std::setw(12) << "P1 Payoff"
              << std::setw(12) << "P2 Payoff"
              << std::setw(10) << "P1 Score"
              << std::setw(10) << "P2 Score"
              << "\033[0m" << "\n";

    std::cout << std::string(92, '-') << "\n";

    for (size_t i = 0; i < gameHistory.size(); ++i)
    {
        const GameRound &round = gameHistory[i];

        std::cout << std::left
                  << std::setw(8) << i + 1

                  << std::setw(20)
                  << (round.playerOneMove == Move::Cooperate
                          ? "Cooperate"
                          : "Defect")

                  << std::setw(20)
                  << (round.playerTwoMove == Move::Cooperate
                          ? "Cooperate"
                          : "Defect")

                  << std::setw(12) << round.playerOnePayOff
                  << std::setw(12) << round.playerTwoPayOff
                  << std::setw(10) << round.playerOneScore
                  << std::setw(10) << round.playerTwoScore

                  << "\n";
    }

    std::cout << std::string(92, '-') << "\n";

    int playerOneFinalScore = playerOne->getScore();
    int playerTwoFinalScore = playerTwo->getScore();

    std::cout << "\033[1m" << "Final Score\n\n"
              << "\033[0m";
    std::cout << std::setw(20) << playerOneName << std::setw(10) << ": " << playerOneFinalScore << "\n";
    std::cout << std::setw(20) << playerTwoName << std::setw(10) << ": " << playerTwoFinalScore << "\n\n";

    // Winner
    std::string winnerName;
    if (playerOneFinalScore > playerTwoFinalScore)
    {
        winnerName = playerOneName;
    }
    else if (playerTwoFinalScore > playerOneFinalScore)
    {
        winnerName = playerTwoName;
    }
    else
    {
        winnerName = "Draw";
    }
    std::cout << std::setw(20) << "Winner" << std::setw(10) << ": " << winnerName << "\n";

    std::cout << "\n"
              << heavyHalfSeperator << "\033[1m" << " END HISTORY " << "\033[0m"
              << heavyHalfSeperator << "\n\n";
}