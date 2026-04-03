#include "control.h"

Control::Control(ProgramState *ps, BallManager *bm, GraphicsData *gd, vector<Button> b)
{
    programState = ps;
    ballManager = bm;
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

    Rectangle simRec = graphicsData->gameStateRecs[GraphicsData::SimRec];
    Rectangle menuRec = graphicsData->gameStateRecs[GraphicsData::MenuRec];

    // click in sim
    if (clicked && CheckCollisionPointRec(mousePos, simRec))
    {
        ballManager->addBallCenter();
    }

    // click in menu
    if (clicked && CheckCollisionPointRec(mousePos, menuRec))
    {
    }

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
}

void Control::buttonsForShop(Vector2 &mousePos, bool clicked) {}