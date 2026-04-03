#pragma once
#include "basicFunctions.h"
#include "raylib.h"
#include <vector>

using std::vector;

class BuildRecs
{
private:
    GraphicsData data;
    vector<Rectangle> buttonRecs;

    void buildMainMenuRecs();
    void buildGameStateRecs();
    void buildShopRecs();

    void buildWallRecs(float wallWidth);

    void buildButtonRecs();

public:
    BuildRecs(float screenWidth, float screenHeight);
    GraphicsData buildGraphicsData();
    inline vector<Rectangle> getButtonRecs() { return buttonRecs; }
};
