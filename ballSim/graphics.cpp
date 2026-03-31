#include "graphics.h"

Graphics::Graphics()
{
    // Set graphics data
    float simPercent = 0.75;
    data.screenRec = {
        0, 0,
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())};
    data.simRec = {
        0, 0,
        data.screenRec.width * simPercent,
        data.screenRec.height};
    data.menuRec = {
        data.simRec.x + data.simRec.width,
        0,
        data.screenRec.width - data.simRec.width,
        data.screenRec.height};

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

    case State::Settings:
        printSettings();
        break;
    }

    DrawFPS(10, 10);
    EndDrawing();
}

// Gets pointer and rectangle wall vector
void Graphics::getBallManagerPtr(BallManager *ballManager)
{
    this->ballManager = ballManager;

    ballWalls = ballManager->getWalls();
}

// private
void Graphics::printMainMenu()
{
    string text = "Ball Simulation";
    int titleFontSize = 20;

    Point titlePoint = centerTextInRec(data.screenRec, text, titleFontSize);

    DrawText(text.c_str(), titlePoint.x, titlePoint.y, titleFontSize, colors[TitleText]);
}

void Graphics::printGame()
{
    // background rectangles
    DrawRectangleRec(data.simRec, colors[SimBackground]);
    DrawRectangleRec(data.menuRec, colors[MenuBackground]);

    // wall rectangles
    for (Rectangle &rec : ballWalls)
    {
        DrawRectangleRec(rec, colors[SimWalls]);
    }

    // draw balls
    printBalls();
}

void Graphics::printSettings()
{
    int horizontalBuffer = 0.20 * data.screenRec.width;
    int verticalBuffer = 0.20 * data.screenRec.height;

    float titleRecPercent = 0.25;

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

    // Draw title
    string titleText = "Settings";
    int titleFontSize = 20;
    Point titlePoint = centerTextInRec(titleRec, titleText, titleFontSize);

    DrawRectangleRec(titleRec, WHITE);
    DrawText(titleText.c_str(), titlePoint.x, titlePoint.y, titleFontSize, colors[SettingsText]);

    // Draw Options
    DrawRectangleRec(optionsRec, GRAY);
}

void Graphics::printBalls()
{
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
