#pragma once

#include "enemyShip.h"

class BomberShip : public EnemyShip
{
public:
    BomberShip(const ShipData &data, World &world, PlayerShip &playerShip,
               const ShipVisuals &visuals, AudioManager &audioManager,
               ProjectileSpawner projectileSpawner)
        : EnemyShip(data, world, FollowType::TouchRandom, playerShip, visuals,
                    audioManager, std::move(projectileSpawner))
    {
    }
};