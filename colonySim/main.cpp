#include "raylib.h"
#include <vector>

#include "tileManager.h"
#include "graphics.h"
#include "entityManager.h"
#include "structureManager.h"
#include "control.h"

using std::vector;

int main()
{
    Rectangle screenRec = {0, 0, 1200, 800};
    InitWindow(screenRec.width, screenRec.height, "Colony Sim");

    Rectangle simRec = {0, 0, screenRec.width, 608};

    GraphicsData gData = {screenRec, simRec};

    Vector2 tileSize = {16, 16};
    Vector2 tileAmount = {simRec.width / tileSize.x, simRec.height / tileSize.y};

    Camera2D camera;
    TileManager tileManager(tileAmount);
    EntityManager entityManager(simRec);
    StructureManager structureManager;
    Graphics graphics(&gData, &tileManager, &entityManager, &structureManager, tileSize, &camera);
    Control control(&camera, &entityManager, &structureManager, &tileManager);

    SetTargetFPS(60);

    int numEntities = 20;
    for (int i = 0; i < numEntities; i++)
    {
        entityManager.createEntity(randomPointInRec(&simRec), {"Entity" + std::to_string(i), (float)100 + i, (float)GetRandomValue(0, 20), true});
    }

    while (!WindowShouldClose())
    {
        control.cameraControls();
        control.debugKeyboardControls();

        // Update
        entityManager.updateEntities();
        graphics.printScreen();
    }

    CloseWindow();

    return 0;
}
