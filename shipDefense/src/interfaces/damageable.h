#pragma once

class Damageable;

enum class Team
{
    Neutral,
    Player,
    Enemy,
};

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

    virtual Team getTeam() const
    {
        return Team::Neutral;
    }

    void dealDamage(Damageable &target, float amount, DamageType type)
    {
        target.takeDamage({amount, type, this});
    }

    virtual void takeDamage(const DamageEvent &damage) = 0;
};