#include "shipManager.h"

#include <stdexcept>
#include <utility>

ShipManager::ShipManager(RenderManager &renderManager, UpdateManager &updateManager,
                         CollisionManager &collisionManager, World &world,
                         TextureManager &textureManager,
                         ProjectileSpawner projectileSpawner)
    : renderManager(renderManager),
      updateManager(updateManager),
      collisionManager(collisionManager),
      world(world),
      textureManager(textureManager),
      projectileSpawner(std::move(projectileSpawner))
{
}

ShipManager::~ShipManager()
{
    clear();
}

Ship &ShipManager::createShip(ShipType type, const ShipData &data)
{
    if (type == ShipType::Player)
    {
        if (playerShip)
            throw std::logic_error("A player ship has already been created");

        playerShip = std::make_unique<PlayerShip>(
            data, world, textureManager.playerVisuals,
            textureManager.pelletAtlas, projectileSpawner);
        registerShip(*playerShip);
        return *playerShip;
    }

    if (playerShip == nullptr)
        throw std::logic_error("A player ship must exist before creating an enemy ship");

    std::unique_ptr<EnemyShip> enemy;
    if (type == ShipType::Bomber)
    {
        enemy = std::make_unique<BomberShip>(
            data, world, *playerShip, textureManager.bomberVisuals,
            projectileSpawner);
    }
    else if (type == ShipType::Gunner)
    {
        enemy = std::make_unique<GunnerShip>(
            data, world, *playerShip, textureManager.fighterVisuals,
            textureManager.torpedoAtlas,
            projectileSpawner);
    }
    else
    {
        throw std::invalid_argument("Unsupported ship type");
    }

    EnemyShip *enemyPointer = enemy.get();
    enemyShips.push_back(std::move(enemy));
    registerShip(*enemyPointer);
    return *enemyPointer;
}

void ShipManager::cleanupDestroyedShips()
{
    for (auto enemy = enemyShips.begin(); enemy != enemyShips.end();)
    {
        if (!(*enemy)->isDead() || !(*enemy)->isDestructionComplete())
        {
            ++enemy;
            continue;
        }

        unregisterShip(**enemy);
        enemy = enemyShips.erase(enemy);

        // TODO implement ship death ramifications
    }

    if (playerShip && playerShip->isDead())
    {
        unregisterShip(*playerShip);
    }
}

void ShipManager::clear()
{
    for (const auto &enemy : enemyShips)
        unregisterShip(*enemy);

    enemyShips.clear();

    if (playerShip)
    {
        unregisterShip(*playerShip);
        playerShip.reset();
    }
}

bool ShipManager::hasNoEnemyShips() const
{
    return enemyShips.empty();
}

PlayerShip *ShipManager::getPlayerShip() const
{
    return playerShip.get();
}

void ShipManager::registerShip(Ship &ship)
{
    updateManager.add(&ship);
    renderManager.add(&ship);
    collisionManager.add(&ship);
}

void ShipManager::unregisterShip(Ship &ship)
{
    updateManager.remove(&ship);
    renderManager.remove(&ship);
    collisionManager.remove(&ship);
}
