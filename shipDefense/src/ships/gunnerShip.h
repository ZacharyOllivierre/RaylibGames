#pragma once

#include "enemyShip.h"
#include "../projectiles/torpedo.h"

#include "raymath.h"

class GunnerShip : public EnemyShip
{
public:
    GunnerShip(const ShipData &data, World &world, PlayerShip &playerShip,
               const ShipVisuals &visuals, Atlas &projectileAtlas, ProjectileSpawner projectileSpawner)
        : EnemyShip(data, world, FollowType::Approach, playerShip, visuals,
                    std::move(projectileSpawner)),
          shotInterval(1.0f),
          timer(shotInterval),
          projectileAtlas(projectileAtlas)
    {
    }

    void update(float delta) override
    {
        timer -= delta;
        if (timer <= 0)
        {
            timer += shotInterval;

            fireProjectile();
        }
        EnemyShip::update(delta);
    }

private:
    void fireProjectile()
    {
        Rectangle body = getCollisionBounds();
        Vector2 center = {
            body.x + body.width / 2.0f,
            body.y + body.height / 2.0f};

        float angle = (data.rotation - 90.0f) * DEG2RAD;
        Vector2 direction = {cosf(angle), sinf(angle)};
        float spawnDistance = data.textureSize.y / 2;

        Vector2 position = Vector2Add(center, Vector2Scale(direction, spawnDistance));
        Vector2 velocity = Vector2Scale(direction, 600.0f);

        spawnProjectile(std::make_unique<Torpedo>(
            position, velocity, *this, projectileAtlas));
    }

private:
    // TODO let this be cusomizable
    float shotInterval;
    float timer;

    Atlas &projectileAtlas;
};