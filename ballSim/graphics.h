#pragma once
#include "ballManager.h"
#include "raylib.h"
#include "basicFunctions.h"

#include <vector>
#include <string>

using std::string;
using std::vector;

struct GraphicsData
{
    Rectangle screenRec;
    Rectangle simRec;
    Rectangle menuRec;
};

enum class State
{
    Main,
    Game,
    Settings
};

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
    void printSettings();

    void printBalls();

    Point centerTextInRec(Rectangle &rec, string &text, int fontSize);

public:
    Graphics();

    void printState(State state);

    // Must be called before use
    void getBallManagerPtr(BallManager *ballManager);

    inline GraphicsData getData() { return data; }
};