#pragma once

#include "raylib.h"

// this is a gross way of solving texture ownership problem
class TextureManager
{
public:
    TextureManager()
    {
        playerShip = LoadTexture("assets/textures/playerShip.png");
        bomberShip = LoadTexture("assets/textures/bomberShip.png");
        gunnerShip = LoadTexture("assets/textures/gunnerShip.png");
    }

    ~TextureManager()
    {
        UnloadTexture(playerShip);
        UnloadTexture(bomberShip);
    }

public:
    Texture2D playerShip;
    Texture2D bomberShip;
    Texture2D gunnerShip;
};