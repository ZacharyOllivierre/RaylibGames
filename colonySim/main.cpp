#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <vector>

#include "tileManager.h"
#include "graphics.h"
using std::vector;

int main()
{
    Rectangle screenRec = {0, 0, 1200, 800};
    InitWindow(screenRec.width, screenRec.height, "Colony Sim");

    Rectangle simRec = {0, 0, screenRec.width, 512};

    GraphicsData gData = {screenRec, simRec};

    Vector2 tileSize = {16, 16};
    Vector2 tileAmount = {simRec.width / tileSize.x, simRec.height / tileSize.y};

    TileManager tileManager(tileAmount);
    Graphics graphics(&gData, &tileManager, tileSize);

    // SetTargetFPS(60);

    srand(time(nullptr));

    while (!WindowShouldClose())
    {
        graphics.printScreen();
    }

    CloseWindow();

    return 0;
}
