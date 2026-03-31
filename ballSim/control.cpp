#include "control.h"

Control::Control(State *state)
{
    this->state = state;
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
    if (IsMouseButtonPressed(1))
    {
        *state = State::Game;
    }
}

void Control::controlsForGame() {}

void Control::controlsForShop() {}