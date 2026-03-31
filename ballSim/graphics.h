#pragma once
#include "ballManager.h"
#include "raylib.h"
#include "basicFunctions.h"

#include <vector>
#include <string>

using std::string;
using std::vector;

enum ColorWheel
{
    Background,
    SimBackground,
    MenuBackground,
    SimWalls,
    TitleText,
    SettingsText,
};

class Graphics
{
private:
    GraphicsData data;
    vector<Color> colors;
    BallManager *ballManager;

    vector<Rectangle> ballWalls;

    void printMainMenu();
    void printGame();
    void printShop();

    void printBalls();

    Point centerTextInRec(Rectangle &rec, string &text, int fontSize);

public:
    Graphics(GraphicsData data, BallManager *ballManager);

    void printState(State state);

    inline GraphicsData getData() { return data; }
};