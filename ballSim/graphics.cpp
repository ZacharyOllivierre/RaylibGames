#include "graphics.h"

Graphics::Graphics(GraphicsData data, BallManager *ballManager)
{
    this->data = data;
    this->ballManager = ballManager;
    ballWalls = data.wallRecs;

    // Set colors
    colors.push_back(BLACK); // background
    colors.push_back(WHITE); // simulation background
    colors.push_back(GRAY);  // menu background
    colors.push_back(BLACK); // Wall color
    colors.push_back(RED);   // title text
    colors.push_back(GREEN); // setting text
}

void Graphics::printState(State state)
{
    BeginDrawing();

    ClearBackground(colors[Background]);

    switch (state)
    {
    case State::Main:
        printMainMenu();
        break;

    case State::Game:
        printGame();
        break;

    case State::Shop:
        printShop();
        break;
    }

    DrawFPS(10, 10);
    EndDrawing();
}

// private
void Graphics::printMainMenu()
{
    string text = "Ball Simulation";
    int titleFontSize = 20;

    Point titlePoint = centerTextInRec(
        data.mainMenuRecs[GraphicsData::MainTitleRec],
        text,
        titleFontSize);

    DrawText(text.c_str(), titlePoint.x, titlePoint.y, titleFontSize, colors[TitleText]);
}

void Graphics::printGame()
{
    // background rectangles
    DrawRectangleRec(data.gameStateRecs[GraphicsData::SimRec], colors[SimBackground]);
    DrawRectangleRec(data.gameStateRecs[GraphicsData::MenuRec], colors[MenuBackground]);

    // wall rectangles
    for (Rectangle &rec : ballWalls)
    {
        DrawRectangleRec(rec, colors[SimWalls]);
    }

    // draw balls
    printBalls();
}

void Graphics::printShop()
{
    string titleText = "Settings";
    int titleFontSize = 20;
    Rectangle titleRec = data.shopRecs[GraphicsData::ShopTitleRec];

    Point titlePoint = centerTextInRec(titleRec, titleText, titleFontSize);

    // Draw title
    DrawRectangleRec(titleRec, WHITE);
    DrawText(titleText.c_str(), titlePoint.x, titlePoint.y, titleFontSize, colors[SettingsText]);

    // Draw Options
    Rectangle optionsRec = data.shopRecs[GraphicsData::OptionsRec];
    DrawRectangleRec(optionsRec, GRAY);
}

void Graphics::printBalls()
{
    if (ballManager == nullptr)
    {
        return;
    }

    for (Ball *b : ballManager->balls)
    {
        DrawCircle(b->point.x, b->point.y, b->radius, rgbToColor(b->ballColor));
    }
}

Point Graphics::centerTextInRec(Rectangle &rec, string &text, int fontSize)
{
    Point p;
    int textWidth = MeasureText(text.c_str(), fontSize);

    p.x = getCenterX(rec) - (textWidth / 2);
    p.y = getCenterY(rec);

    return p;
}
