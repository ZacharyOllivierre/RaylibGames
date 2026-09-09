#include "gameViewer.h"

#include "raylib.h"

GameViewer::GameViewer(Game &game)
    : game(game), currentRound(0)
{
    // Init viewer data
    data.screenWidth = GetScreenWidth();
    data.screenHeight = GetScreenHeight();
    data.centerX = data.screenWidth / 2;
    data.leftX = data.screenWidth / 4;
    data.rightX = (data.screenWidth * 3) / 4;
    data.titleFontSize = 60;
    data.playerFontSize = 40;
    data.normalFontSize = 35;
    data.smallFontSize = 25;

    data.history = game.getGameHistory();
}

void GameViewer::step()
{
    currentRound += 1;

    // If on post game screen reset to beginning of game
    if (currentRound > data.history.size())
    {
        currentRound = 0;
    }
}

bool GameViewer::isFinished() const
{
    return currentRound >= data.history.size();
}

// Draw dispatcher
void GameViewer::draw() const
{
    if (data.history.empty())
        return;

    if (isFinished())
    {
        drawPostGame();
    }
    else
    {
        drawDuringGame();
    }
}

void GameViewer::drawDuringGame() const
{
    auto drawCentered = [](const char *text, int x, int y, int fontSize, Color color)
    {
        int width = MeasureText(text, fontSize);
        DrawText(text, x - width / 2, y, fontSize, color);
    };

    const GameRound &round = data.history[currentRound];

    drawCentered(
        "PRISONER'S DILEMMA",
        data.centerX,
        data.screenHeight * 0.08f,
        data.titleFontSize, WHITE);

    drawCentered(
        TextFormat("Round %d / %d", currentRound + 1, static_cast<int>(data.history.size())),
        data.centerX,
        data.screenHeight * 0.20f,
        data.normalFontSize, LIGHTGRAY);

    drawCentered(
        game.getPlayerOne()->getName().c_str(),
        data.leftX,
        data.screenHeight * 0.32f,
        data.playerFontSize, SKYBLUE);

    drawCentered(
        game.getPlayerTwo()->getName().c_str(),
        data.rightX,
        data.screenHeight * 0.32f,
        data.playerFontSize, SKYBLUE);

    const char *playerOneMove = round.playerOneMove == Move::Cooperate
                                    ? "COOPERATE"
                                    : "DEFECT";
    const char *playerTwoMove = round.playerTwoMove == Move::Cooperate
                                    ? "COOPERATE"
                                    : "DEFECT";

    drawCentered(
        playerOneMove,
        data.leftX,
        data.screenHeight * 0.45f,
        data.playerFontSize, WHITE);

    drawCentered(
        playerTwoMove,
        data.rightX,
        data.screenHeight * 0.45f,
        data.playerFontSize, WHITE);

    drawCentered(
        TextFormat("Payoff: %d", round.playerOnePayOff),
        data.leftX,
        data.screenHeight * 0.56f,
        data.smallFontSize, LIGHTGRAY);

    drawCentered(
        TextFormat("Payoff: %d", round.playerTwoPayOff),
        data.rightX,
        data.screenHeight * 0.56f,
        data.smallFontSize, LIGHTGRAY);

    drawCentered(
        TextFormat("Score: %d", round.playerOneScore),
        data.leftX,
        data.screenHeight * 0.67f,
        data.normalFontSize, GREEN);

    drawCentered(
        TextFormat("Score: %d", round.playerTwoScore),
        data.rightX,
        data.screenHeight * 0.67f,
        data.normalFontSize, GREEN);
}

void GameViewer::drawPostGame() const
{
    DrawText(
        "Game Over",
        data.centerX - MeasureText("Game Over", data.titleFontSize) / 2,
        data.screenHeight * 0.10f,
        data.titleFontSize, WHITE);

    drawGameHistoryTable();
}

// Should refactor to add scrolling
void GameViewer::drawGameHistoryTable() const
{
    const int rowHeight = 32;

    const int tableWidth = data.screenWidth * 0.90f;
    const int startX = (data.screenWidth - tableWidth) / 2;
    const int startY = data.screenHeight * 0.30f;

    const int roundWidth = tableWidth * 0.08f;
    const int playerWidth = tableWidth * 0.20f;
    const int payoffWidth = tableWidth * 0.12f;
    const int scoreWidth = tableWidth * 0.14f;

    int x = startX;

    // Header
    DrawText("Round", x, startY, data.smallFontSize, WHITE);
    x += roundWidth;

    DrawText(
        game.getPlayerOne()->getName().c_str(),
        x, startY, data.smallFontSize, SKYBLUE);
    x += playerWidth;

    DrawText(
        game.getPlayerTwo()->getName().c_str(),
        x, startY, data.smallFontSize, SKYBLUE);
    x += playerWidth;

    DrawText("P1 Payoff", x, startY, data.smallFontSize, WHITE);
    x += payoffWidth;

    DrawText("P2 Payoff", x, startY, data.smallFontSize, WHITE);
    x += payoffWidth;

    DrawText("P1 Score", x, startY, data.smallFontSize, WHITE);
    x += scoreWidth;

    DrawText("P2 Score", x, startY, data.smallFontSize, WHITE);

    // Header line
    DrawLine(
        startX,
        startY + rowHeight - 5,
        startX + tableWidth,
        startY + rowHeight - 5,
        GRAY);

    // Rounds
    int maxRounds = 12;
    for (size_t i = 0; i < data.history.size(); i++)
    {
        if (i > maxRounds)
            break;

        const GameRound &round = data.history[i];

        int y = startY + rowHeight * (i + 1);
        x = startX;

        DrawText(
            TextFormat("%d", static_cast<int>(i + 1)),
            x, y, data.smallFontSize, LIGHTGRAY);
        x += roundWidth;

        DrawText(
            round.playerOneMove == Move::Cooperate
                ? "Cooperate"
                : "Defect",
            x, y, data.smallFontSize,
            round.playerOneMove == Move::Cooperate
                ? GREEN
                : RED);
        x += playerWidth;

        DrawText(
            round.playerTwoMove == Move::Cooperate
                ? "Cooperate"
                : "Defect",
            x, y, data.smallFontSize,
            round.playerTwoMove == Move::Cooperate
                ? GREEN
                : RED);
        x += playerWidth;

        DrawText(
            TextFormat("%d", round.playerOnePayOff),
            x, y, data.smallFontSize, WHITE);
        x += payoffWidth;

        DrawText(
            TextFormat("%d", round.playerTwoPayOff),
            x, y, data.smallFontSize, WHITE);
        x += payoffWidth;

        DrawText(
            TextFormat("%d", round.playerOneScore),
            x, y, data.smallFontSize, GREEN);
        x += scoreWidth;

        DrawText(
            TextFormat("%d", round.playerTwoScore),
            x, y, data.smallFontSize, GREEN);
    }

    // Final score
    if (!game.getGameHistory().empty())
    {
        const GameRound &finalRound = data.history.back();
        // TODO gross
        int y = startY + rowHeight * maxRounds + 65;

        DrawLine(
            startX, y,
            startX + tableWidth, y,
            GRAY);
        x = startX;
        y += 1;

        DrawText("FINAL", x, y, data.smallFontSize, GOLD);
        x += roundWidth;

        DrawText("SCORE", x, y, data.smallFontSize, GOLD);
        x += (playerWidth * 2) + (payoffWidth * 2);

        DrawText(
            TextFormat("%d", finalRound.playerOneScore),
            x, y, data.smallFontSize, GOLD);
        x += scoreWidth;

        DrawText(
            TextFormat("%d", finalRound.playerTwoScore),
            x, y, data.smallFontSize, GOLD);
    }
}
