#pragma once

#include "game.h"

struct ViewerData
{
    int screenWidth;
    int screenHeight;

    int centerX;
    int leftX;
    int rightX;

    int titleFontSize;
    int playerFontSize;
    int normalFontSize;
    int smallFontSize;

    std::vector<GameRound> history;
};

class GameViewer
{
public:
    GameViewer(Game &game);

    void step();
    void draw() const;

    bool isFinished() const;

private:
    void drawDuringGame() const;
    void drawPostGame() const;

    void drawGameHistoryTable() const;

private:
    Game &game;
    int currentRound;

    ViewerData data;
};
