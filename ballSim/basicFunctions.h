#pragma once

#include "raylib.h"
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

struct Point
{
    float x;
    float y;
};

struct RGB
{
    unsigned char r, g, b, a;
};

enum class State
{
    Main,
    Game,
    Shop
};

struct ProgramState
{
    State state = State::Main;
    bool paused = false;
};

// this is bad
struct GraphicsData
{
    Rectangle screenRec;

    enum MMRec
    {
        MainTitleRec = 0,
        SubtitleRec = 1
    };
    vector<Rectangle> mainMenuRecs;

    enum GSRec
    {
        SimRec = 0,
        MenuRec = 1,
    };
    vector<Rectangle> gameStateRecs;
    vector<Rectangle> wallRecs;

    enum SRec
    {
        ShopTitleRec = 0,
        OptionsRec = 1
    };
    vector<Rectangle> shopRecs;
};

// Rectangle helpers
int getCenterX(Rectangle &rec);
int getCenterY(Rectangle &rec);
Point getCenter(Rectangle &rec);

// Color
Color rgbToColor(RGB color);

// Debugging
void dPrintRectangle(Rectangle &rec);
