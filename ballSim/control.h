#pragma once
#include "basicFunctions.h"
#include "ballManager.h"
#include "raylib.h"
#include "button.h"

class Control
{
private:
    ProgramState *programState;
    BallManager *ballManager;
    GraphicsData *graphicsData;

    vector<Button> buttons;

    void controlsForMain();
    void controlsForGame();
    void controlsForShop();

    void buttonsForGame(Vector2 &mousePos, bool clicked);
    void buttonsForShop(Vector2 &mousePos, bool clicked);

public:
    Control(ProgramState *ps, BallManager *bm, GraphicsData *gd, vector<Button> b);

    void controlsForState(State s);
};