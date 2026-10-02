#pragma once

#include "raylib.h"

struct CollisionContact
{
    Vector2 normal;
    float penetration;
};

class Collidable
{
public:
    virtual ~Collidable() = default;

    virtual Rectangle getCollisionBounds() const = 0;
    virtual void onCollision(Collidable &other,
                             const CollisionContact &contact) = 0;
};