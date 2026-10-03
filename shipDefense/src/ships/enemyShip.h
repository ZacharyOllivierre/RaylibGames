#pragma once

#include "ship.h"
#include "playerShip.h"

enum class FollowType
{
    Touch,
    TouchRandom,
    Approach,
};

class EnemyShip : public Ship
{
public:
    EnemyShip(const ShipData &data, World &world, FollowType type,
              PlayerShip &playerShip, const ShipVisuals &visuals,
              ProjectileSpawner projectileSpawner);

    void update(float delta) override;

private:
    void applyFollowForce();
    void basicFollow(float &xForce, float &yForce);

    FollowType followType;
    PlayerShip &playerShip;
};