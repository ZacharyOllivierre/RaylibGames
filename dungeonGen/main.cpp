#include <iostream>
#include "raylib.h"

#include "graphics.h"
#include "mapGenerator.h"
#include "waveFC.h"
#include "tileMap.h"

using namespace std;

const Vector2 screenDimensions = {800, 800};
const Vector2 gridDimensions = {20, 20};

int main()
{
    InitWindow(screenDimensions.x, screenDimensions.y, "WFC");
    SetTargetFPS(60);

    TileMap grid;
    MapGenerator mapGenerator(gridDimensions, MapConfig{}, grid);
    WaveFC wave(grid);
    Graphics graphics(&mapGenerator, &wave, screenDimensions, gridDimensions);

    const char *sampleImagePath = "assets/wfc_sample.png";
    if (FileExists(sampleImagePath))
    {
        wave.loadSampleImage(sampleImagePath);
    }

    mapGenerator.generateRoom();
    wave.rebuild();

    bool lifted = true;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_TAB))
        {
            graphics.toggleView();
        }

        if (IsKeyDown(KEY_SPACE) && lifted)
        {
            mapGenerator.generateRoom();
            wave.rebuild();
            lifted = false;
        }
        if (IsKeyReleased(KEY_SPACE))
        {
            lifted = true;
        }

        if (!wave.finished())
        {
            wave.step();
        }

        // if (IsKeyDown(KEY_W))
        // {
        //     wave.step();
        // }

        graphics.draw();
    }

    return 0;
}