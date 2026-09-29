#pragma once

#include "ship.h"
#include "playerShip.h"

// TODO using elsewhere but not here
enum class EnemyType
{
    Bomber,
};

enum class FollowType
{
    Touch,
    TouchRandom,
};

// TODO change to sub classes of types of enemies

class EnemyShip : public Ship
{
public:
    EnemyShip(const ShipData &data, World &world, Sprite sprite,
              FollowType type, PlayerShip &pShip, EnemyType enemyType);

    void update(float delta) override;

private:
    void applyFollowForce();
    void basicFollow(float &xForce, float &yForce);

private:
    EnemyType enemyType;
    FollowType followType;

    PlayerShip &pShip;
};