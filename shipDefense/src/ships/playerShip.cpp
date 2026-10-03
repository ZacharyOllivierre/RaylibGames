#include "playerShip.h"

#include "../projectiles/pellet.h"

#include "raymath.h"

#include <cmath>

PlayerShip::PlayerShip(const ShipData &data, World &world,
                       const ShipVisuals &visuals, Atlas &projectileAtlas,
                       ProjectileSpawner projectileSpawner)
    : Ship(data, world, visuals, std::move(projectileSpawner)),
      projectileAtlas(projectileAtlas)
{
}

void PlayerShip::update(float delta)
{
    shipInputControls();

    Ship::update(delta);
}

void PlayerShip::fireProjectile()
{
    Rectangle body = getCollisionBounds();
    Vector2 center = {
        body.x + body.width / 2.0f,
        body.y + body.height / 2.0f};

    float angle = (data.rotation - 90.0f) * DEG2RAD;
    Vector2 direction = {cosf(angle), sinf(angle)};
    float spawnDistance = data.textureSize.y / 2.0f + 8.0f;

    Vector2 position = Vector2Add(center, Vector2Scale(direction, spawnDistance));
    Vector2 velocity = Vector2Scale(direction, 800.0f);

    spawnProjectile(std::make_unique<Pellet>(
        position, velocity, *this, projectileAtlas));
}

void PlayerShip::shipInputControls()
{
    if (IsKeyDown(KEY_W))
        addForce({0, -data.speed});
    if (IsKeyDown(KEY_S))
        addForce({0, data.speed});
    if (IsKeyDown(KEY_A))
        addForce({-data.speed, 0});
    if (IsKeyDown(KEY_D))
        addForce({data.speed, 0});

    if (IsKeyPressed(KEY_SPACE))
        fireProjectile();
}