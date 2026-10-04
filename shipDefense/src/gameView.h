#pragma once

#include "raylib.h"

#include "sceneTypes.h"

struct GameView
{
    int currentRound;
    int maxRounds;
    float playerHealth;
    float playerMaxHealth;
    Vector2 cameraTarget;
    GameResult result;
};