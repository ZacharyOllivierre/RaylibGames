#include "raylib.h"
#include "ballManager.h"
#include "graphics.h"
#include "control.h"
#include "buildRecs.h"

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "ball sim");
    SetTargetFPS(60);

    State state = State::Game;
    GraphicsData data;
    BuildRecs recs(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    data = recs.buildGraphicsData();

    BallManager ballManager(data.gameStateRecs[GraphicsData::SimRec], data.wallRecs);
    Graphics graphics(data, &ballManager);

    // Control control(&state);

    while (!WindowShouldClose())
    {
        graphics.printState(state);
        // control.controlsForState(state);

        if (state == State::Game)
        {
            ballManager.updateBalls();
        }
    }

    CloseWindow();
    return 0;
}
