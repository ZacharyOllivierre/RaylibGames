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

    case FollowType::Approach:

        float approachDistance = 200;
        Vector2 playerPos = pShip.getPosition();
        Vector2 shipPos = Ship::getPosition();

        float dx = playerPos.x - shipPos.x;
        float dy = playerPos.y - shipPos.y;

        float distanceSquared = dx * dx + dy * dy;

        // approach to a point
        if (distanceSquared > approachDistance * approachDistance)
        {
            basicFollow(xForce, yForce);
        }
        // back away within approach distance
        else
        {
            float length = std::sqrt(distanceSquared);

            if (length > 0.0f)
            {
                dx /= length;
                dy /= length;

                xForce = -dx * data.speed;
                yForce = -dy * data.speed;
            }
        }

        break;
    }

    addForce({xForce, yForce});
}

void EnemyShip::basicFollow(float &xForce, float &yForce)
{
    Vector2 playerPos = pShip.getPosition();
    Vector2 shipPos = Ship::getPosition();

    Vector2 direction = {playerPos.x - shipPos.x,
                         playerPos.y - shipPos.y};

    float length = std::sqrt(direction.x * direction.x +
                             direction.y * direction.y);

    if (length > 0.0f)
    {
        direction.x /= length;
        direction.y /= length;
    }

    xForce = direction.x * data.speed;
    yForce = direction.y * data.speed;
}