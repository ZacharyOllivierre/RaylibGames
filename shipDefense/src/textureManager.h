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
        starBackground = LoadTexture("assets/textures/stars.png");
    }

    ~TextureManager()
    {
        UnloadTexture(playerShip);
        UnloadTexture(bomberShip);
        UnloadTexture(gunnerShip);
        UnloadTexture(starBackground);
    }

public:
    Texture2D playerShip;
    Texture2D bomberShip;
    Texture2D gunnerShip;

    Texture2D starBackground;
};