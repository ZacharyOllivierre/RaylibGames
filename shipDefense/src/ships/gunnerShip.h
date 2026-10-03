#pragma once

#include "enemyShip.h"

class GunnerShip : public EnemyShip
{
public:
    GunnerShip(const ShipData &data, World &world, PlayerShip &playerShip,
               const ShipVisuals &visuals, ProjectileSpawner projectileSpawner)
        : EnemyShip(data, world, FollowType::Approach, playerShip, visuals,
                    std::move(projectileSpawner))
    {
    }
};