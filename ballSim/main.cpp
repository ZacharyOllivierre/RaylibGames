#include "raylib.h"
#include "ballManager.h"
#include "graphics.h"
#include "control.h"
#include "buildRecs.h"
#include "player.h"
#include "button.h"

void initButtons(vector<Button> &buttons, BuildRecs *br);

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "ball sim");
    SetTargetFPS(60);

    // Init data structs
    ProgramState programState;
    GraphicsData data;
    vector<Button> buttons;

    // Init and build graphics rectangles
    BuildRecs recs(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    // Set data from buildRecs
    data = recs.buildGraphicsData();
    initButtons(buttons, &recs);

    // Init main classes with data
    Player player;
    BallManager ballManager(data.gameStateRecs[GraphicsData::SimRec], data.wallRecs, &player);
    Graphics graphics(data, &ballManager, &player, buttons);
    Control control(&programState, &ballManager, &data, buttons);

    while (!WindowShouldClose())
    {
        graphics.printState(programState.state);
        control.controlsForState(programState.state);

        // update balls if in game state and not paused
        if (programState.state == State::Game && !programState.paused)
        {
            ballManager.updateBalls();
        }
    }

    CloseWindow();
    return 0;
}

void initButtons(vector<Button> &buttons, BuildRecs *br)
{
    vector<Rectangle> buttonRecs = br->getButtonRecs();

    // Game menu to main menu button
    buttons.push_back(
        {buttonRecs[GameToMainMenu],
         GameToMainMenu,
         "Go to Main Menu",
         false});

    // Game menu to shop
    buttons.push_back({buttonRecs[GameToShop],
                       GameToShop,
                       "Go to Shop",
                       false});
}