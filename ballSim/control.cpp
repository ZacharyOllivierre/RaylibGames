#include "control.h"
#include <iostream>

Control::Control(ProgramState *ps, BallManager *bm, GraphicsData *gd, Player *p, Shop *s, vector<Button> *b)
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
    if (IsKeyPressed(KEY_SPACE))
    {
        programState->state = State::Game;
    }
}

void Control::controlsForGame()
{
    Vector2 mousePos = GetMousePosition();

    // spacebar
    if (IsKeyPressed(KEY_SPACE))
    {
        // invert pause
        programState->paused == true ? programState->paused = false : programState->paused = true;
    }

    buttonsForGame(mousePos);
}

void Control::controlsForShop()
{
    Vector2 mousePos = GetMousePosition();

    buttonsForShop(mousePos);
}

void Control::buttonsForGame(Vector2 &mousePos)
{
    bool down = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[GameToMainMenu].rec))
    {
        programState->state = State::Main;
        (*buttons)[GameToMainMenu].isPressed = true;
    }
    else
    {
        (*buttons)[GameToMainMenu].isPressed = false;
    }

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[GameToShop].rec))
    {
        programState->state = State::Shop;
        (*buttons)[GameToShop].isPressed = true;
    }
    else
    {
        (*buttons)[GameToShop].isPressed = false;
    }

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[GameJoltBalls].rec))
    {
        if (clicked)
        {
            int joltCost = 15;
            ballManager->joltBalls();
            player->subtractPoints(joltCost);
        }

        (*buttons)[GameJoltBalls].isPressed = true;
    }
    else
    {
        (*buttons)[GameJoltBalls].isPressed = false;
    }
}

// this function is a sin please repent - fix
void Control::buttonsForShop(Vector2 &mousePos)
{
    bool down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[ShopToGame].rec))
    {
        programState->state = State::Game;
        (*buttons)[ShopToGame].isPressed = true;
    }
    else
    {
        (*buttons)[ShopToGame].isPressed = false;
    }

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[ShopAddBall].rec))
    {
        if (clicked && shop->purchaseItem(ShopItem::AddBall))
        {
            ballManager->addBallCenter();
        }
        (*buttons)[ShopAddBall].isPressed = true;
    }
    else
    {
        (*buttons)[ShopAddBall].isPressed = false;
    }

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[ShopIncreaseBounce].rec))
    {
        if (clicked && shop->purchaseItem(ShopItem::IncreaseBounce))
        {
            ballManager->bounceCoefficient += 0.01;
        }
        (*buttons)[ShopIncreaseBounce].isPressed = true;
    }
    else
    {
        (*buttons)[ShopIncreaseBounce].isPressed = false;
    }

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[ShopReduceFriction].rec))
    {
        if (clicked && shop->purchaseItem(ShopItem::ReduceFriction))
        {
            ballManager->friction += 0.01;
        }
        (*buttons)[ShopReduceFriction].isPressed = true;
    }
    else
    {
        (*buttons)[ShopReduceFriction].isPressed = false;
    }

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[ShopReduceGravity].rec))
    {
        if (clicked && shop->purchaseItem(ShopItem::ReduceGravity))
        {
            ballManager->gravity -= 0.1;
        }
        (*buttons)[ShopReduceGravity].isPressed = true;
    }
    else
    {
        (*buttons)[ShopReduceGravity].isPressed = false;
    }

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[ShopIncreaseJolt].rec))
    {
        if (shop->purchaseItem(ShopItem::JoltPercent))
        {
            ballManager->joltPercent += 0.05;
        }
        (*buttons)[ShopIncreaseJolt].isPressed = true;
    }
    else
    {
        (*buttons)[ShopIncreaseJolt].isPressed = false;
    }

    if (down && CheckCollisionPointRec(mousePos, (*buttons)[ShopIncreaseGravity].rec))
    {
        if (shop->purchaseItem(ShopItem::IncreaseGravity))
        {
            ballManager->gravity += 0.2;
        }
        (*buttons)[ShopIncreaseGravity].isPressed = true;
    }
    else
    {
        (*buttons)[ShopIncreaseGravity].isPressed = false;
    }
}