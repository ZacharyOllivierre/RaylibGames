#pragma once

#include "raylib.h"

// this is a gross way of solving texture ownership problem
class TextureManager
{
public:
    TextureManager()
    {
        playerShip = LoadTexture("assets/textures/playerShip.png");
        enemyShip = LoadTexture("assets/textures/enemyShip.png");
    }

    ~TextureManager()
    {
        UnloadTexture(playerShip);
        UnloadTexture(enemyShip);
    }

public:
    Texture2D playerShip;
    Texture2D enemyShip;
};