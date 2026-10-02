#pragma once

class Collidable;

enum class DamageType
{
    Contact,
    Projectile,
};

struct DamageEvent
{
    float amount;
    DamageType type;
    Collidable *source;
};

class Damageable
{
public:
    virtual ~Damageable() = default;

    virtual void takeDamage(const DamageEvent &damage) = 0;
};