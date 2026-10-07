#pragma once

#include "projectile.h"

class Pellet : public Projectile
{
public:
    Pellet(Vector2 position, Vector2 velocity, Damageable &owner,
           Atlas &atlas)
        : Projectile(position, velocity, {16, 32}, {16, 32}, atlas, owner, 30.0f)
    {
    }
};