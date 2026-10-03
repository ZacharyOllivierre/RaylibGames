#pragma once

class Damageable;

enum class DamageType
{
    Contact,
    Projectile,
};

struct DamageEvent
{
    float amount;
    DamageType type;
    Damageable *source;
};

class Damageable
{
public:
    virtual ~Damageable() = default;

    void dealDamage(Damageable &target, float amount, DamageType type)
    {
        target.takeDamage({amount, type, this});
    }

    virtual void takeDamage(const DamageEvent &damage) = 0;
};