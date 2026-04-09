#include "buildRecs.h"

BuildRecs::BuildRecs(float screenWidth, float screenHeight)
{
    data.screenRec = {0, 0, screenWidth, screenHeight};
    buildGraphicsData();
    buildButtonRecs();
}

GraphicsData BuildRecs::buildGraphicsData()
{
    buildMainMenuRecs();
    buildGameStateRecs();
    buildShopRecs();

    return data;
}

void BuildRecs::buildMainMenuRecs()
{
    float titlePercent = 0.45;

    Rectangle titleRec = {
        0,
        0,
        data.screenRec.width,
        data.screenRec.height * titlePercent};

    Rectangle subtitleRec = {
        0,
        titleRec.y + titleRec.height,
        data.screenRec.width,
        data.screenRec.height - titleRec.height};

    data.mainMenuRecs.push_back(titleRec);
    data.mainMenuRecs.push_back(subtitleRec);
}

void BuildRecs::buildGameStateRecs()
{
    float simPercent = 0.75;
    Rectangle simRec = {
        0, 0,
        data.screenRec.width * simPercent,
        data.screenRec.height};

    Rectangle menuRec = {
        simRec.x + simRec.width,
        0,
        data.screenRec.width - simRec.width,
        data.screenRec.height};

    data.gameStateRecs.push_back(simRec);
    data.gameStateRecs.push_back(menuRec);

    float wallWidth = 10;
    buildWallRecs(wallWidth);
}

void BuildRecs::buildShopRecs()
{
    int horizontalBuffer = 0.05 * data.screenRec.width;
    int verticalBuffer = 0.05 * data.screenRec.height;

    float titleRecPercent = 0.15;

    Rectangle titleRec = {
        data.screenRec.x + horizontalBuffer,
        data.screenRec.y + verticalBuffer,
        data.screenRec.width - horizontalBuffer * 2,
        data.screenRec.height * titleRecPercent - verticalBuffer};

    Rectangle optionsRec = {
        titleRec.x,
        titleRec.y + titleRec.height,
        titleRec.width,
        data.screenRec.height - (verticalBuffer * 2) - titleRec.height};

    data.shopRecs.push_back(titleRec);
    data.shopRecs.push_back(optionsRec);
}

void BuildRecs::buildWallRecs(float wallWidth)
{
    Rectangle ballRec = data.gameStateRecs[GraphicsData::SimRec];

    Rectangle leftWall = {
        ballRec.x,
        ballRec.y,
        wallWidth,
        ballRec.height};

    Rectangle topWall = {
        ballRec.x,
        ballRec.y,
        ballRec.width,
        wallWidth};

    Rectangle rightWall = {
        ballRec.x + ballRec.width - wallWidth,
        ballRec.y,
        wallWidth,
        ballRec.height};

    Rectangle bottomWall = {
        ballRec.x,
        ballRec.y + ballRec.height - wallWidth,
        ballRec.width,
        wallWidth};

    data.wallRecs.push_back(leftWall);
    data.wallRecs.push_back(topWall);
    data.wallRecs.push_back(rightWall);
    data.wallRecs.push_back(bottomWall);
}

void BuildRecs::buildButtonRecs()
{
    // game menu to main menu
    Rectangle *menuRec = &data.gameStateRecs[GraphicsData::MenuRec];
    buttonRecs.push_back({menuRec->x,
                          menuRec->y + menuRec->height * 0.9f,
                          menuRec->width,
                          menuRec->height - menuRec->height * 0.9f});
    // game to shop
    buttonRecs.push_back({buttonRecs[0].x,
                          buttonRecs[0].y - buttonRecs[0].height,
                          buttonRecs[0].width,
                          buttonRecs[0].height});
    // game jolt balls
    buttonRecs.push_back({menuRec->x,
                          menuRec->y,
                          menuRec->width,
                          buttonRecs[0].height});
    // shop to game
    Rectangle *shopOpRec = &data.shopRecs[GraphicsData::OptionsRec];
    buttonRecs.push_back({shopOpRec->x,
                          shopOpRec->y + shopOpRec->height * .90f,
                          shopOpRec->width * .20f,
                          shopOpRec->height * .10f});

    int shopItemHorSpace = 40;
    int shopItemVirSpace = 40;
    int numCols = 3;

    float itemWidth = (shopOpRec->width - (numCols - 1) * shopItemHorSpace) / (float)numCols;
    float itemHeight = shopOpRec->height * 0.25f;

    float startX = shopOpRec->x + (shopOpRec->width - (numCols * itemWidth + (numCols - 1) * shopItemHorSpace)) / 2;
    float startY = shopOpRec->y + shopItemVirSpace;

    for (int col = 0; col < numCols; col++)
    {
        Rectangle rec = {
            startX + col * (itemWidth + shopItemHorSpace),
            startY,
            itemWidth,
            itemHeight};
        buttonRecs.push_back(rec);
    }

    float row2Y = startY + itemHeight + shopItemVirSpace;
    for (int col = 0; col < numCols; col++)
    {
        Rectangle rec = {
            startX + col * (itemWidth + shopItemHorSpace),
            row2Y,
            itemWidth,
            itemHeight};
        buttonRecs.push_back(rec);
    }
}