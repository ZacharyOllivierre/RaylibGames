#include "raylib.h"

#include "updateManager.h"
#include "collisionManager.h"
#include "renderManager.h"
#include "textureManager.h"

#include "graphics.h"
#include "world.h"
#include "game.h"

#include "sceneTypes.h"

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
                      &renderManager, &textureManager);

    Game game(&renderManager, &updateManager, &collisionManager, &world,
              &graphics, &textureManager);
    updateManager.add(&game);

    Scene scene = Scene::MainMenu;

    while (!WindowShouldClose())
    {

        graphics.draw(scene, game.getResult());

        switch (scene)
        {
        case Scene::MainMenu:
            if (IsKeyPressed(KEY_SPACE))
            {
                game.resetGame();
                scene = Scene::Game;
            }
            break;

        case Scene::Game:
        {
            float delta = GetFrameTime();
            updateManager.update(delta);
            collisionManager.checkCollisions();

            if (game.getResult() != GameResult::InProgress)
                scene = Scene::GameOver;

            break;
        }

        case Scene::GameOver:
            if (IsKeyPressed(KEY_SPACE))
            {
                // duplicate
                game.resetGame();
                scene = Scene::MainMenu;
            }

            break;
        }
    }

    CloseWindow();

    return 0;
}