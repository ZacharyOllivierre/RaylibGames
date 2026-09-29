#include "enemyShip.h"

EnemyShip::EnemyShip(const ShipData &data, World &world, Sprite sprite,
                     FollowType type, PlayerShip &pShip, EnemyType enemyType)
    : Ship(data, world, sprite), enemyType(enemyType), followType(type), pShip(pShip)
{
}

void EnemyShip::update(float delta)
{
    applyFollowForce();

    Ship::update(delta);
}

void EnemyShip::applyFollowForce()
{
    float xForce = 0;
    float yForce = 0;

    switch (followType)
    {
    case FollowType::Touch:

        basicFollow(xForce, yForce);
        break;

    // TODO replace the "random"
    case FollowType::TouchRandom:
        basicFollow(xForce, yForce);

        if (GetRandomValue(0, 10) == 0)
        {
            xForce *= GetRandomValue(0, 2);
            yForce *= GetRandomValue(0, 2);
        }
        break;

    default:
        return;
    }

    addForce({xForce, yForce});
}

void EnemyShip::basicFollow(float &xForce, float &yForce)
{
    Vector2 playerPos = pShip.getPosition();
    Vector2 shipPos = Ship::getPosition();

    // x update
    if (playerPos.x > shipPos.x)
    {
        xForce = data.speed;
    }
    else
    {
        xForce = -data.speed;
    }
    // y update
    if (playerPos.y > shipPos.y)
    {
        yForce = data.speed;
    }
    else
    {
        yForce = -data.speed;
    }
}