#include "raylib.h"
#include <vector>

#include "tileManager.h"
#include "graphics.h"
#include "entityManager.h"

#include <iostream>
using namespace std;

using std::vector;

int main()
{
    Rectangle screenRec = {0, 0, 1200, 800};
    InitWindow(screenRec.width, screenRec.height, "Colony Sim");

    Rectangle simRec = {0, 0, screenRec.width, 560};

    GraphicsData gData = {screenRec, simRec};

    Vector2 tileSize = {16, 16};
    Vector2 tileAmount = {simRec.width / tileSize.x, simRec.height / tileSize.y};

    TileManager tileManager(tileAmount);
    EntityManager entityManager(simRec);
    Graphics graphics(&gData, &tileManager, &entityManager, tileSize);

    SetTargetFPS(60);

    entityManager.createEntity({100, 100}, "Entity1", 500, 5);
    entityManager.createEntity({333, 300}, "Entity2", 500, 1.5);

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 clickPos = GetMousePosition();
            entityManager.getRandomEntity()->moveToPos(clickPos);
        }

        // if (IsKeyPressed(KEY_BACKSPACE))
        // {
        //     cout << "Entity count before kill " << entityManager.entityList.size() << endl;
        //     entityManager.killEntity(entityManager.getRandomEntity());
        //     cout << "Entity count after kill " << entityManager.entityList.size() << endl;
        // }

        entityManager.updateEntities();
        graphics.printScreen();
    }

    CloseWindow();

    return 0;
}
