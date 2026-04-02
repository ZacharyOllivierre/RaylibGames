#include "control.h"

Control::Control(ProgramState *ps, BallManager *bm, GraphicsData *gd)
{
    programState = ps;
    ballManager = bm;
    graphicsData = gd;
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
    if (clicked && pointWithinRec(mousePos, simRec))
    {
        ballManager->addBallCenter();
    }

    // click in menu
    if (clicked && pointWithinRec(mousePos, menuRec))
    {
    }

    // spacebar
    if (IsKeyPressed(KEY_SPACE))
    {
        // invert pause
        programState->paused == true ? programState->paused = false : programState->paused = true;
    }
}

void Control::controlsForShop() {}
