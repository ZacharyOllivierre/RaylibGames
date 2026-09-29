#include "enemyShip.h"

EnemyShip::EnemyShip(const ShipData &data, World &world, Sprite sprite,
                     FollowType type, PlayerShip &pShip)
    : Ship(data, world, sprite), followType(type), pShip(pShip)
{
}

void EnemyShip::update(float delta)
{
    applyFollowForce();

    Ship::update(delta);
}

void EnemyShip::applyFollowForce()
{
    Vector2 shipPos = Ship::getPosition();
    Vector2 playerPos = pShip.getPosition();

    switch (followType)
    {
    case FollowType::Touch:

        // x update
        if (playerPos.x > shipPos.x)
        {
            addForce({data.speed, 0});
        }
        else
        {
            addForce({-data.speed, 0});
        }

        // y update
        if (playerPos.y > shipPos.y)
        {
            addForce({0, data.speed});
        }
        else
        {
            addForce({0, -data.speed});
        }
        break;

    default:
        return;
    }
}