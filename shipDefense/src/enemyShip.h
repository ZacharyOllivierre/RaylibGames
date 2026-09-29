#pragma once

#include "ship.h"
#include "playerShip.h"

enum class EnemyType
{
    Bomber,
};

enum class FollowType
{
    Touch,
};

class EnemyShip : public Ship
{
public:
    EnemyShip(const ShipData &data, World &world, Sprite sprite,
              FollowType type, PlayerShip &pShip);

    void update(float delta) override;

private:
    void applyFollowForce();

private:
    FollowType followType;

    PlayerShip &pShip;
};