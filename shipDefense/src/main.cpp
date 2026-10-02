#include "raylib.h"

#include "updateManager.h"
#include "collisionManager.h"
#include "renderManager.h"
#include "textureManager.h"

#include "graphics.h"
#include "world.h"
#include "game.h"

const Vector2 WORLD_SIZE = {2500, 2500};
const Vector2 WINDOW_SIZE = {1300, 800};

int main()
{
    InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Ship Defender or Something");
    SetTargetFPS(60);

    UpdateManager updateManager;
    CollisionManager collisionManager;
    RenderManager renderManager;
    TextureManager textureManager;

    World world(WORLD_SIZE);

    Graphics graphics({(float)GetScreenWidth(),
                       (float)GetScreenHeight()},
                      WORLD_SIZE,
                      &renderManager);

    Game game(&renderManager, &updateManager, &collisionManager, &world,
              &graphics, &textureManager);
    updateManager.add(&game);

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        updateManager.update(delta);
        collisionManager.checkCollisions();
        game.cleanupDestroyedShips();

        graphics.drawGame();
    }

    CloseWindow();

    return 0;
}