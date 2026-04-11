#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <vector>

#include "tileManager.h"
#include "graphics.h"
#include "entityManager.h"

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

    srand(time(nullptr));

    entityManager.createEntity();

    while (!WindowShouldClose())
    {
        Vector2 clickPos;
        bool clicked = false;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            clickPos = GetMousePosition();
            clicked = true;
        }
        if (clicked)
        {
            entityManager.entityList[0]->moveToPos(clickPos);
        }

        if (rand() % 5 == 0)
        {
            entityManager.updateEntities();
            graphics.printScreen();
        }
    }

    CloseWindow();

    return 0;
}
