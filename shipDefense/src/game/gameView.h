#pragma once

#include "raylib.h"
#include <vector>
#include <string>

#include "../sceneTypes.h"

struct GameView
{
    int currentRound;
    int maxRounds;
    float playerHealth;
    float playerMaxHealth;
    Vector2 cameraTarget;
    GameResult result;

    bool upgradeWindowActive;
};