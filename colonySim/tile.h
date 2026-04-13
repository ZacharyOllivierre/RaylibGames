#pragma once
#include "raylib.h"

class Tile
{
private:
    Vector2 position;

public:
    Tile(Vector2 p);

    inline Vector2 *getPosition() { return &position; }
};