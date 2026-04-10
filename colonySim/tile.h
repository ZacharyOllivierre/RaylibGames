#pragma once
#include "raylib.h"

class Tile
{
private:
    Color color;
    Vector2 position;

public:
    Tile(Color c, Vector2 p);

    inline Vector2 *getPosition() { return &position; }
    inline Color *getColor() { return &color; }
};