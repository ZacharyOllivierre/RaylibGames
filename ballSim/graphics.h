#pragma once
#include "ballManager.h"
#include "raylib.h"
#include "basicFunctions.h"
#include "button.h"

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
    ScoreText,
};

class Graphics
{
private:
    GraphicsData data;

    BallManager *ballManager;
    Player *player;
    vector<Button> buttons;

    vector<Rectangle> ballWalls;
    vector<Color> colors;

    void printMainMenu();
    void printGame();
    void printShop();

    void printBalls();
    void printScore();

    void printButton(Button button);

    Point centerTextInRec(Rectangle &rec, string &text, int fontSize);

public:
    Graphics(GraphicsData data, BallManager *ballManager, Player *player, vector<Button> buttons);

    void printState(State state);

    inline GraphicsData getData() { return data; }
};