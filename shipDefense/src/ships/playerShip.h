#pragma once

#include "raylib.h"

#include "ship.h"

class Atlas;

class PlayerShip : public Ship
{
public:
    PlayerShip(const ShipData &data, World &world, const ShipVisuals &visuals,
               Atlas &projectileAtlas, ProjectileSpawner projectileSpawner);

    void update(float delta) override;

    bool isPlayerShip() const override
    {
        return true;
    }

private:
    void shipInputControls();
    void fireProjectile();

    Atlas &projectileAtlas;
};