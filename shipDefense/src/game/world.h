#pragma once

#include "raylib.h"

class PhysicsBody;

class World
{
public:
    World(Vector2 dimensions);

    void constrain(PhysicsBody &body, bool wrap = false);

    Vector2 getDimensions() const;
    Rectangle getBounds() const;

private:
    Vector2 dimensions;
};
