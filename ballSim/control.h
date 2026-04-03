#pragma once
#include "basicFunctions.h"
#include "ballManager.h"
#include "raylib.h"
#include "button.h"
#include "player.h"
#include "shop.h"

class Control
{
private:
    ProgramState *programState;
    BallManager *ballManager;
    Player *player;
    Shop *shop;
    GraphicsData *graphicsData;

    vector<Button> buttons;

    void controlsForMain();
    void controlsForGame();
    void controlsForShop();

    void buttonsForGame(Vector2 &mousePos, bool clicked);
    void buttonsForShop(Vector2 &mousePos, bool clicked);

public:
    Control(ProgramState *ps, BallManager *bm, GraphicsData *gd, Player *p, Shop *s, vector<Button> b);

    void controlsForState(State s);
};