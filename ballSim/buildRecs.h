#pragma once
#include "basicFunctions.h"
#include "raylib.h"

class BuildRecs
{
private:
    GraphicsData data;

    void buildMainMenuRecs();
    void buildGameStateRecs();
    void buildShopRecs();

    void buildWallRecs(float wallWidth);

public:
    BuildRecs(float screenWidth, float screenHeight);
    GraphicsData buildGraphicsData();
};
