#pragma once

#include "raylib.h"

#include "ship.h"

class PlayerShip : public Ship
{
public:
    PlayerShip(const ShipData &data, World &world, Sprite sprite);

    void update(float delta) override;

    bool isCollisionImmovable() const override
    {
        return true;
    }

    bool isPlayerShip() const override
    {
        return true;
    }

private:
    void shipInputControls();

private:
};