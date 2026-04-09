#pragma once
#include "raylib.h"
#include <string>

using std::string;

enum ButtonType
{
    GameToMainMenu,
    GameToShop,
    GameJoltBalls,
    ShopToGame,
    ShopAddBall,
    ShopIncreaseBounce,
    ShopReduceFriction,
    ShopReduceGravity,
    ShopIncreaseJolt,
    ShopIncreaseGravity,
};

struct Button
{
    Rectangle rec;
    ButtonType buttonType;
    string label;
    bool isPressed;
};