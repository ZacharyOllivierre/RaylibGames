#include "control.h"
#include <iostream>

Control::Control(ProgramState *ps, BallManager *bm, GraphicsData *gd, Player *p, Shop *s, vector<Button> b)
{
    programState = ps;
    ballManager = bm;
    player = p;
    shop = s;
    graphicsData = gd;
    buttons = b;
}

void Control::controlsForState(State s)
{
    switch (s)
    {
    case State::Main:
        controlsForMain();
        break;
    case State::Game:
        controlsForGame();
        break;
    case State::Shop:
        controlsForShop();
        break;
    }
}

void Control::controlsForMain()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        programState->state = State::Game;
    }
}

void Control::controlsForGame()
{
    Vector2 mousePos = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    // Rectangle simRec = graphicsData->gameStateRecs[GraphicsData::SimRec];

    // // click in sim
    // if (clicked && CheckCollisionPointRec(mousePos, simRec))
    // {
    //     ballManager->addBallCenter();
    // }

    // spacebar
    if (IsKeyPressed(KEY_SPACE))
    {
        // invert pause
        programState->paused == true ? programState->paused = false : programState->paused = true;
    }

    // buttons
    buttonsForGame(mousePos, clicked);
}

void Control::controlsForShop()
{
    Vector2 mousePos = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    buttonsForShop(mousePos, clicked);
}

void Control::buttonsForGame(Vector2 &mousePos, bool clicked)
{
    if (clicked && CheckCollisionPointRec(mousePos, buttons[GameToMainMenu].rec))
    {
        programState->state = State::Main;
        buttons[GameToMainMenu].isPressed = true;
    }
    else
    {
        buttons[GameToMainMenu].isPressed = false;
    }

    if (clicked && CheckCollisionPointRec(mousePos, buttons[GameToShop].rec))
    {
        programState->state = State::Shop;
        buttons[GameToShop].isPressed = true;
    }
    else
    {
        buttons[GameToShop].isPressed = false;
    }

    if (clicked && CheckCollisionPointRec(mousePos, buttons[GameJoltBalls].rec))
    {
        int joltCost = 20;
        ballManager->joltBalls();
        player->subtractPoints(joltCost);

        buttons[GameJoltBalls].isPressed = true;
    }
    else
    {
        buttons[GameJoltBalls].isPressed = false;
    }
}

void Control::buttonsForShop(Vector2 &mousePos, bool clicked)
{
    if (clicked && CheckCollisionPointRec(mousePos, buttons[ShopToGame].rec))
    {
        programState->state = State::Game;
        buttons[ShopToGame].isPressed = true;
    }
    else
    {
        buttons[ShopToGame].isPressed = false;
    }

    if (clicked && CheckCollisionPointRec(mousePos, buttons[ShopAddBall].rec))
    {
        if (shop->purchaseItem(ShopItem::AddBall))
        {
            ballManager->addBallCenter();
            buttons[ShopAddBall].isPressed = true;
        }
    }
    else
    {
        buttons[ShopAddBall].isPressed = false;
    }

    if (clicked && CheckCollisionPointRec(mousePos, buttons[ShopIncreaseBounce].rec))
    {
        if (shop->purchaseItem(ShopItem::IncreaseBounce))
        {
            ballManager->bounceCoefficient += 0.01;
            buttons[ShopIncreaseBounce].isPressed = true;
        }
    }
    else
    {
        buttons[ShopIncreaseBounce].isPressed = false;
    }

    if (clicked && CheckCollisionPointRec(mousePos, buttons[ShopReduceFriction].rec))
    {
        if (shop->purchaseItem(ShopItem::ReduceFriction))
        {
            ballManager->friction += 0.01;
            buttons[ShopReduceFriction].isPressed = true;
        }
    }
    else
    {
        buttons[ShopReduceFriction].isPressed = false;
    }

    if (clicked && CheckCollisionPointRec(mousePos, buttons[ShopReduceGravity].rec))
    {
        if (shop->purchaseItem(ShopItem::ReduceGravity))
        {
            ballManager->gravity -= 0.1;
            buttons[ShopReduceGravity].isPressed = true;
        }
    }
    else
    {
        buttons[ShopReduceGravity].isPressed = false;
    }

    if (clicked && CheckCollisionPointRec(mousePos, buttons[ShopIncreaseJolt].rec))
    {
        if (shop->purchaseItem(ShopItem::JoltPercent))
        {
            ballManager->joltPercent += 0.05;
            buttons[ShopIncreaseJolt].isPressed = true;
        }
    }
    else
    {
        buttons[ShopIncreaseJolt].isPressed = false;
    }
}