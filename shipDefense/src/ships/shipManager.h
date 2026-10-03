#pragma once

#include <memory>
#include <vector>

#include "bomberShip.h"
#include "gunnerShip.h"
#include "playerShip.h"
#include "../collisionManager.h"
#include "../renderManager.h"
#include "../textureManager.h"
#include "../updateManager.h"

class ShipManager
{
public:
    enum class ShipType
    {
        Player,
        Bomber,
        Gunner
    };

    ShipManager(RenderManager &renderManager, UpdateManager &updateManager,
                CollisionManager &collisionManager, World &world,
                TextureManager &textureManager,
                ProjectileSpawner projectileSpawner);
    ~ShipManager();

    Ship &createShip(ShipType type, const ShipData &data);
    void cleanupDestroyedShips();
    void clear();
    bool hasNoEnemyShips() const;

    PlayerShip *getPlayerShip() const;

private:
    void registerShip(Ship &ship);
    void unregisterShip(Ship &ship);

    RenderManager &renderManager;
    UpdateManager &updateManager;
    CollisionManager &collisionManager;
    World &world;
    TextureManager &textureManager;
    ProjectileSpawner projectileSpawner;

    std::unique_ptr<PlayerShip> playerShip;
    std::vector<std::unique_ptr<EnemyShip>> enemyShips;
};
