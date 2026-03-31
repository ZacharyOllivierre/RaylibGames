#pragma once
#include "basicFunctions.h"
#include "raylib.h"

class Control
{
private:
    State *state;

    void controlsForMain();
    void controlsForGame();
    void controlsForShop();

public:
    Control(State *state);

    void controlsForState(State s);
};