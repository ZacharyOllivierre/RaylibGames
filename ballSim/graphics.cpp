#include "graphics.h"

Graphics::Graphics(GraphicsData data, BallManager *ballManager, Player *player, vector<Button> buttons)
{
    this->data = data;
    this->ballManager = ballManager;
    this->player = player;
    ballWalls = data.wallRecs;

    this->buttons = buttons;

    // Set colors
    colors.push_back(BLACK);  // background
    colors.push_back(WHITE);  // simulation background
    colors.push_back(GRAY);   // menu background
    colors.push_back(BLACK);  // Wall color
    colors.push_back(RED);    // title text
    colors.push_back(GREEN);  // setting text
    colors.push_back(YELLOW); // score text
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
    string subText = "Click to continue";
    int titleFontSize = 20;

    Point titlePoint = centerTextInRec(
        data.mainMenuRecs[GraphicsData::MainTitleRec],
        text,
        titleFontSize);

    Point subTitlePoint = centerTextInRec(
        data.mainMenuRecs[GraphicsData::SubtitleRec],
        subText,
        titleFontSize);

    DrawText(
        text.c_str(),
        titlePoint.x,
        titlePoint.y,
        titleFontSize,
        colors[TitleText]);

    DrawText(
        subText.c_str(),
        subTitlePoint.x,
        subTitlePoint.y,
        titleFontSize,
        colors[TitleText]);
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

    printScore();

    printButton(buttons[GameToMainMenu]);
    printButton(buttons[GameToShop]);
}

void Graphics::printShop()
{
    string titleText = "Shop";
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

void Graphics::printScore()
{
    int fontSize = 30;

    string text = "Score: ";
    text += std::to_string(player->getPoints());

    Rectangle scoreRec = data.gameStateRecs[GraphicsData::SimRec];
    scoreRec.height = fontSize + (fontSize / 2);

    Point scorePoint = centerTextInRec(scoreRec, text, fontSize);

    DrawText(text.c_str(), scorePoint.x, scorePoint.y, fontSize, colors[ScoreText]);
}

void Graphics::printButton(Button button)
{
    string text = button.label;
    int fontsize = 20;
    Point textPoint = centerTextInRec(button.rec, text, fontsize);

    DrawRectangleRec(button.rec, RED);
    DrawText(text.c_str(), textPoint.x, textPoint.y, fontsize, WHITE);
}

Point Graphics::centerTextInRec(Rectangle &rec, string &text, int fontSize)
{
    Point p;
    int textWidth = MeasureText(text.c_str(), fontSize);

    p.x = getCenterX(rec) - (textWidth / 2);
    p.y = getCenterY(rec);

    return p;
}
