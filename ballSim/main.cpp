#include "raylib.h"
#include "ballManager.h"
#include "graphics.h"
#include "control.h"
#include "buildRecs.h"
#include "player.h"
#include "button.h"
#include "shop.h"

void initButtons(vector<Button> &buttons, BuildRecs *br);

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Bounce");
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
    Shop shop(&player);
    BallManager ballManager(data.gameStateRecs[GraphicsData::SimRec], data.wallRecs, &player);
    Graphics graphics(data, &ballManager, &player, &shop, &buttons);
    Control control(&programState, &ballManager, &data, &player, &shop, &buttons);

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
    buttons.push_back({buttonRecs[GameToMainMenu],
                       GameToMainMenu, "Go to Main Menu", false});
    // Game menu to shop
    buttons.push_back({buttonRecs[GameToShop],
                       GameToShop, "Go to Shop", false});
    // Game jolt balls
    buttons.push_back({buttonRecs[GameJoltBalls],
                       GameJoltBalls, "Jolt Balls", false});
    // Shop to game
    buttons.push_back({buttonRecs[ShopToGame],
                       ShopToGame, "Go to Game", false});
    // Shop Item add ball
    buttons.push_back({buttonRecs[ShopAddBall],
                       ShopAddBall, "Buy Ball", false});
    // Shop item increase bounce
    buttons.push_back({buttonRecs[ShopIncreaseBounce],
                       ShopIncreaseBounce, "Upgrade Bounce", false});
    // Shop item reduce friction
    buttons.push_back({buttonRecs[ShopReduceFriction],
                       ShopReduceFriction, "Reduce Friction", false});
    // Shop item reduce gravity
    buttons.push_back({buttonRecs[ShopReduceGravity],
                       ShopReduceGravity, "Reduce Gravity", false});
    // Shop increase jolt
    buttons.push_back({buttonRecs[ShopIncreaseJolt],
                       ShopIncreaseJolt, "Upgrade Jolt", false});
    // Shop increase gravity
    buttons.push_back({buttonRecs[ShopIncreaseGravity],
                       ShopIncreaseGravity, "Increase Gravity", false});
}