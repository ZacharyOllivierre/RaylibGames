#pragma once

#include "projectile.h"

class Torpedo : public Projectile
{
public:
    Torpedo(Vector2 position, Vector2 velocity, Damageable &owner,
            Atlas &atlas)
        : Projectile(position, velocity, {30, 20}, {32, 64}, atlas, owner, 10.0f, 10.0f) {}
};