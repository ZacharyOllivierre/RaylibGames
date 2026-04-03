#pragma once
#include "raylib.h"
#include <string>

using std::string;

enum ButtonType
{
    GameToMainMenu = 0,
    GameToShop = 1,
};

struct Button
{
    Rectangle rec;
    ButtonType buttonType;
    string label;
    bool isPressed;
};