#include "graphics.h"

Graphics::Graphics(GraphicsData data, BallManager *ballManager, Player *player, Shop *shop, vector<Button> *buttons)
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

    canvas = LoadRenderTexture(data.screenRec.width, data.screenRec.height);
}

Graphics::~Graphics()
{
    UnloadRenderTexture(canvas);
}

void Graphics::printState(State state)
{
    // to add dynamic window sizing graphics data needs to not be copied by value - fix
    // create scene in texture and paste texture onto screen rec
    BeginTextureMode(canvas);

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

    EndTextureMode();

    // draw texture to screen
    BeginDrawing();
    ClearBackground(colors[Background]);

    Rectangle sourceRec = {0, 0, (float)canvas.texture.width, -(float)canvas.texture.height};
    Rectangle destRec = {0, 0, (float)data.screenRec.width, (float)data.screenRec.height};

    DrawTexturePro(
        canvas.texture,
        sourceRec,
        destRec,
        {0.0f, 0.0f},
        0.0f,
        WHITE);
    EndDrawing();
}

// private
void Graphics::printMainMenu()
{
    string text = "Bounce";
    string subText = "Spacebar to Continue";
    int titleFontSize = 80;

    Point titlePoint = centerTextInRec(
        data.mainMenuRecs[GraphicsData::MainTitleRec],
        text,
        titleFontSize);

    Point subTitlePoint = centerTextInRec(
        data.mainMenuRecs[GraphicsData::SubtitleRec],
        subText,
        titleFontSize / 2);

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
        titleFontSize / 2,
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

    printButton((*buttons)[GameToMainMenu]);
    printButton((*buttons)[GameToShop]);
    printButton((*buttons)[GameJoltBalls]);
}

void Graphics::printShop()
{
    int currentScore = player->getPoints();

    string titleText = "Shop - Coins : " + std::to_string(currentScore);
    int titleFontSize = 60;
    Rectangle titleRec = data.shopRecs[GraphicsData::ShopTitleRec];

    Point titlePoint = centerTextInRec(titleRec, titleText, titleFontSize);

    // Draw title
    DrawRectangleRec(titleRec, WHITE);
    DrawText(titleText.c_str(), titlePoint.x, titlePoint.y, titleFontSize, colors[SettingsText]);

    // Draw Options
    Rectangle optionsRec = data.shopRecs[GraphicsData::OptionsRec];
    DrawRectangleRec(optionsRec, GRAY);

    // print back button
    printButton((*buttons)[ShopToGame]);

    // print shop item buttons
    printButton((*buttons)[ShopAddBall]);
    printButton((*buttons)[ShopIncreaseBounce]);
    printButton((*buttons)[ShopReduceFriction]);
    printButton((*buttons)[ShopReduceGravity]);
    printButton((*buttons)[ShopIncreaseJolt]);
    printButton((*buttons)[ShopIncreaseGravity]);

    // print prices for items
    int fontSize = 25;
    int spaceBelow = 3;
    centerTextBelowRecPrint((*buttons)[ShopAddBall].rec,
                            shop->getPriceS(ShopItem::AddBall),
                            fontSize,
                            spaceBelow);
    centerTextBelowRecPrint((*buttons)[ShopIncreaseBounce].rec,
                            shop->getPriceS(ShopItem::IncreaseBounce),
                            fontSize,
                            spaceBelow);
    centerTextBelowRecPrint((*buttons)[ShopReduceFriction].rec,
                            shop->getPriceS(ShopItem::ReduceFriction),
                            fontSize,
                            spaceBelow);
    centerTextBelowRecPrint((*buttons)[ShopReduceGravity].rec,
                            shop->getPriceS(ShopItem::ReduceGravity),
                            fontSize,
                            spaceBelow);
    centerTextBelowRecPrint((*buttons)[ShopIncreaseJolt].rec,
                            shop->getPriceS(ShopItem::JoltPercent),
                            fontSize,
                            spaceBelow);
    centerTextBelowRecPrint((*buttons)[ShopIncreaseGravity].rec,
                            shop->getPriceS(ShopItem::IncreaseGravity),
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
    int fontSize = 50;

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
    int fontsize = 30;
    int borderThickness = 5;
    Point textPoint = centerTextInRec(button.rec, text, fontsize);

    Color curColor = colors[ButtonBase];
    if (button.isPressed)
    {
        curColor = darkenColor(curColor, .6);
    }

    DrawRectangleRec(button.rec, curColor);
    DrawRectangleLinesEx(button.rec, borderThickness, curColor);

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

Color Graphics::darkenColor(Color c, float factor)
{
    Color result;
    result.a = c.a;
    result.r = c.r * factor;
    result.g = c.g * factor;
    result.b = c.b * factor;
    return result;
}
