#include "raylib.h"
#include <vector>

#include "tileManager.h"
#include "graphics.h"
#include "entityManager.h"

using std::vector;

int main()
{
    Rectangle screenRec = {0, 0, 1200, 800};
    InitWindow(screenRec.width, screenRec.height, "Colony Sim");

    Rectangle simRec = {0, 0, screenRec.width, 608};

    GraphicsData gData = {screenRec, simRec};

    Vector2 tileSize = {16, 16};
    Vector2 tileAmount = {simRec.width / tileSize.x, simRec.height / tileSize.y};

    TileManager tileManager(tileAmount);
    EntityManager entityManager(simRec);
    Graphics graphics(&gData, &tileManager, &entityManager, tileSize);

    SetTargetFPS(60);

    int numEntities = 20;
    for (int i = 0; i < numEntities; i++)
    {
        entityManager.createEntity(randomPointInRec(&simRec), {"Entity" + std::to_string(i), (float)100 + i, (float)GetRandomValue(0, 20), true});
    }

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Entity *entity = entityManager.getRandomEntity();

            if (entity != nullptr)
            {
                Vector2 clickPos = GetMousePosition();
                entity->moveToPos(clickPos);
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            Entity *entity = entityManager.getRandomEntity();
            if (entity != nullptr)
            {
                entityManager.killEntity(entity);
            }
        }

        entityManager.updateEntities();
        graphics.printScreen();
    }

    CloseWindow();

    return 0;
}
