#include "enemyShip.h"

EnemyShip::EnemyShip(const ShipData &data, World &world, FollowType type,
                     PlayerShip &playerShip,
                     const ShipVisuals &visuals,
                     ProjectileSpawner projectileSpawner)
    : Ship(data, world, visuals, std::move(projectileSpawner)),
      followType(type), playerShip(playerShip)
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
    {
        basicFollow(xForce, yForce);
        break;
    }

    case FollowType::TouchRandom:
    {
        basicFollow(xForce, yForce);
        if (GetRandomValue(0, 10) == 0)
        {
            xForce *= GetRandomValue(0, 2);
            yForce *= GetRandomValue(0, 2);
        }
        break;
    }

    case FollowType::Approach:
    {
        float approachDistance = 200;
        Vector2 playerPosition = playerShip.getPosition();
        Vector2 shipPosition = getPosition();
        float dx = playerPosition.x - shipPosition.x;
        float dy = playerPosition.y - shipPosition.y;
        float distanceSquared = dx * dx + dy * dy;

        if (distanceSquared > approachDistance * approachDistance)
        {
            basicFollow(xForce, yForce);
        }
        else
        {
            float length = std::sqrt(distanceSquared);
            if (length > 0.0f)
            {
                xForce = -dx / length * data.speed;
                yForce = -dy / length * data.speed;
            }
        }
        break;
    }
    }

    addForce({xForce, yForce});
}

void EnemyShip::basicFollow(float &xForce, float &yForce)
{
    Vector2 playerPosition = playerShip.getPosition();
    Vector2 shipPosition = getPosition();
    Vector2 direction = {playerPosition.x - shipPosition.x,
                         playerPosition.y - shipPosition.y};
    float length = std::sqrt(direction.x * direction.x +
                             direction.y * direction.y);

    if (length <= 0.0f)
    {
        xForce = 0.0f;
        yForce = 0.0f;
        return;
    }

    direction.x = direction.x / length * data.speed;
    direction.y = direction.y / length * data.speed;

    Vector2 velocity = getVelocity();
    xForce = direction.x - velocity.x;
    yForce = direction.y - velocity.y;
}