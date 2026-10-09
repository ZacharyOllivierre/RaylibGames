#include "playerShip.h"

#include "../projectiles/pellet.h"

#include "raymath.h"

#include <cmath>

// TODO starting projectile damage set here and not "init-able"
PlayerShip::PlayerShip(const ShipData &data, World &world,
                       const ShipVisuals &visuals, Atlas &projectileAtlas,
                       AudioManager &audioManager,
                       ProjectileSpawner projectileSpawner)
    : Ship(data, world, visuals, audioManager, std::move(projectileSpawner)),
      projectileAtlas(projectileAtlas),
      projectileDamage(25),
      projectileCooldown(0.8f)
{
}

void PlayerShip::applyUpgrade(UpgradeTypes type)
{
    if (type == UpgradeTypes::ProjectileDamage)
    {
        projectileDamage += 10.0f;
        return;
    }

    else if (type == UpgradeTypes::ProjectileCooldown)
    {
        if (projectileCooldown > 0.1f)
            projectileCooldown -= 0.1f;
    }

    Ship::applyUpgrade(type);
}

void PlayerShip::update(float delta)
{
    shipInputControls();

    if (projectileCooldown >= 0)
    {
        projectileCounter -= delta;
    }

    Ship::update(delta);
}

void PlayerShip::fireProjectile()
{
    if (projectileCounter > 0)
        return;
    projectileCounter = projectileCooldown;
    audioManager.playEffect(AudioManager::Effect::PlayerShoot);

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
        position, velocity, *this, projectileAtlas, projectileDamage));
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

    if (IsKeyDown(KEY_SPACE))
        fireProjectile();
}