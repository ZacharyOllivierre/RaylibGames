#include "raylib.h"

#include "game/game.h"

int main()
{
    const Vector2 renderDimensions = {960, 540};

    InitWindow(renderDimensions.x, renderDimensions.y, "Capitol Bet");
    ToggleFullscreen();
    SetTargetFPS(60);

    Game game({static_cast<float>(GetScreenWidth()),
               static_cast<float>(GetScreenHeight())},
              1.0f);

    while (!WindowShouldClose())
        game.update(GetFrameTime());

    CloseWindow();
    return 0;
}
