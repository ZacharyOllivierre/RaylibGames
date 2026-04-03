#include "graphics.h"

Graphics::Graphics(GraphicsData data, BallManager *ballManager, Player *player, Shop *shop, vector<Button> buttons)
{
    this->data = data;
    this->ballManager = ballManager;
    this->player = player;
    this->shop = shop;
    ballWalls = data.wallRecs;

    this->buttons = buttons;

    // Set colors
    colors.push_back(BLACK);     // background
    colors.push_back(BLACK);     // simulation background
    colors.push_back(DARKGRAY);  // menu background
    colors.push_back(WHITE);     // Wall color
    colors.push_back(RED);       // title text
    colors.push_back(GREEN);     // setting text
    colors.push_back(YELLOW);    // score text
    colors.push_back(BLACK);     // button text
    colors.push_back(RED);       // button base
    colors.push_back(LIGHTGRAY); // button border
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
    printButton(buttons[GameJoltBalls]);
}

void Graphics::printShop()
{
    int currentScore = player->getPoints();

    string titleText = "Shop - Coins : " + std::to_string(currentScore);
    int titleFontSize = 20;
    Rectangle titleRec = data.shopRecs[GraphicsData::ShopTitleRec];

    Point titlePoint = centerTextInRec(titleRec, titleText, titleFontSize);

    // Draw title
    DrawRectangleRec(titleRec, WHITE);
    DrawText(titleText.c_str(), titlePoint.x, titlePoint.y, titleFontSize, colors[SettingsText]);

    // Draw Options
    Rectangle optionsRec = data.shopRecs[GraphicsData::OptionsRec];
    DrawRectangleRec(optionsRec, GRAY);

    // print back button
    printButton(buttons[ShopToGame]);

    // print shop item buttons
    printButton(buttons[ShopAddBall]);
    printButton(buttons[ShopIncreaseBounce]);
    printButton(buttons[ShopReduceFriction]);
    printButton(buttons[ShopReduceGravity]);
    printButton(buttons[ShopIncreaseJolt]);

    // print prices for items
    int fontSize = 20;
    int spaceBelow = 3;
    centerTextBelowRecPrint(buttons[ShopAddBall].rec,
                            shop->getPriceS(ShopItem::AddBall),
                            fontSize,
                            spaceBelow);
    centerTextBelowRecPrint(buttons[ShopIncreaseBounce].rec,
                            shop->getPriceS(ShopItem::IncreaseBounce),
                            fontSize,
                            spaceBelow);
    centerTextBelowRecPrint(buttons[ShopReduceFriction].rec,
                            shop->getPriceS(ShopItem::ReduceFriction),
                            fontSize,
                            spaceBelow);
    centerTextBelowRecPrint(buttons[ShopReduceGravity].rec,
                            shop->getPriceS(ShopItem::ReduceGravity),
                            fontSize,
                            spaceBelow);
    centerTextBelowRecPrint(buttons[ShopIncreaseJolt].rec,
                            shop->getPriceS(ShopItem::JoltPercent),
                            fontSize,
                            spaceBelow);
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

    string text = "Coins: ";
    text += std::to_string(player->getPoints());

    Rectangle scoreRec = data.gameStateRecs[GraphicsData::SimRec];
    scoreRec.height = fontSize + (fontSize / 1.5);

    Point scorePoint = centerTextInRec(scoreRec, text, fontSize);

    DrawText(text.c_str(), scorePoint.x, scorePoint.y, fontSize, colors[ScoreText]);
}

void Graphics::printButton(Button button)
{
    string text = button.label;
    int fontsize = 20;
    int borderThickness = 5;
    Point textPoint = centerTextInRec(button.rec, text, fontsize);

    DrawRectangleRec(button.rec, colors[ButtonBase]);
    DrawRectangleLinesEx(button.rec, borderThickness, colors[ButtonBorder]);

    DrawText(text.c_str(), textPoint.x, textPoint.y, fontsize, colors[ButtonText]);
}

Point Graphics::centerTextInRec(Rectangle &rec, string text, int fontSize)
{
    Font font = GetFontDefault();
    Point p;
    int spacing = 1;
    Vector2 textWidth = MeasureTextEx(font, text.c_str(), fontSize, spacing);

    p.x = (int)(getCenterX(rec) - textWidth.x / 2.0f);
    p.y = (int)(getCenterY(rec) - textWidth.y / 2.0f);

    return p;
}

void Graphics::centerTextBelowRecPrint(Rectangle &rec, string text, int fontSize, int spacingBelow)
{
    Font font = GetFontDefault();
    Point p;
    int spacing = 1;
    Vector2 textDim = MeasureTextEx(font, text.c_str(), fontSize, spacing);

    p.x = getCenterX(rec) - (textDim.x / 2);
    p.y = rec.y + rec.height + spacingBelow;

    DrawTextEx(font, text.c_str(), {p.x, p.y}, fontSize, spacing, colors[ButtonText]);
}