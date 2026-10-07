#include "raylib.h"

#include "updateManager.h"
#include "collisionManager.h"
#include "renderManager.h"
#include "textureManager.h"

#include "graphics/graphics.h"
#include "game/world.h"
#include "game/game.h"

#include "sceneTypes.h"

const Vector2 WORLD_SIZE = {3000, 1500};
Vector2 WINDOW_SIZE = {1500, 900};

int main()
{

    InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Ships And Stuff");
    ToggleFullscreen();

    // TODO not a const
    WINDOW_SIZE = {(float)GetScreenWidth(), (float)GetScreenHeight()};

    SetTargetFPS(60);

    UpdateManager updateManager;
    CollisionManager collisionManager;
    RenderManager renderManager;
    TextureManager textureManager;

    // TODO world should be apart of game
    World world(WORLD_SIZE);

    Game game(&renderManager, &updateManager, &collisionManager, &world,
              &textureManager);
    updateManager.add(&game);

    Graphics graphics(WINDOW_SIZE, WORLD_SIZE,
                      &renderManager, &textureManager, &game);

    Scene scene = Scene::MainMenu;

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        graphics.update(scene, delta);

        switch (scene)
        {
        case Scene::MainMenu:
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                game.resetGame();
                scene = Scene::Game;
                graphics.resetMainMenu();
            }
            break;
        }
        case Scene::Game:
        {
            updateManager.update(delta);
            collisionManager.checkCollisions();

            if (game.getResult() != GameResult::InProgress)
                scene = Scene::GameOver;

            break;
        }
        case Scene::GameOver:
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                // duplicate
                game.resetGame();
                scene = Scene::MainMenu;
            }
            break;
        }
        }
    }

    CloseWindow();

    return 0;
}