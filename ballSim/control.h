#pragma once
#include "basicFunctions.h"
#include "ballManager.h"
#include "raylib.h"

class Control
{
private:
    ProgramState *programState;
    BallManager *ballManager;
    GraphicsData *graphicsData;

    void controlsForMain();
    void controlsForGame();
    void controlsForShop();

public:
    Control(ProgramState *ps, BallManager *bm, GraphicsData *gd);

    void controlsForState(State s);
};