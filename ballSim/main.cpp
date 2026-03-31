#include "raylib.h"
#include "ballManager.h"
#include "graphics.h"

int main()
{
    InitWindow(1000, 800, "ball sim");
    SetTargetFPS(60);

    Graphics graphics;
    BallManager ballManager(graphics.getData().simRec);

    graphics.getBallManagerPtr(&ballManager);
    State state = State::Game;

    while (!WindowShouldClose())
    {
        // TEMP to slow down game
        if (rand() % 10 == 0)
        {
            graphics.printState(state);
            ballManager.updateBalls();
        }
    }

    CloseWindow();
    return 0;
}
