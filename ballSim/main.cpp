#include "raylib.h"
#include "ballManager.h"
#include "graphics.h"
#include "control.h"
#include "buildRecs.h"
#include "player.h"

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "ball sim");
    SetTargetFPS(60);

    // Init data structs
    ProgramState programState;
    GraphicsData data;

    // Init and build graphics rectangles
    BuildRecs recs(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    // Set data from buildRecs
    data = recs.buildGraphicsData();

    // Init main classes with data
    Player player;
    BallManager ballManager(data.gameStateRecs[GraphicsData::SimRec], data.wallRecs, &player);
    Graphics graphics(data, &ballManager, &player);
    Control control(&programState, &ballManager, &data);

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
