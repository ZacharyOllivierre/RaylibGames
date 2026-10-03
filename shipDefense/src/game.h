#pragma once

#include <memory>
#include <vector>

#include "interfaces/updateable.h"

#include "ships/playerShip.h"
#include "ships/bomberShip.h"
#include "ships/gunnerShip.h"
#include "projectiles/projectileManager.h"
#include "renderManager.h"
#include "collisionManager.h"
#include "updateManager.h"
#include "textureManager.h"
#include "graphics.h"

// TODO
// textures being stored in the ships themselves is a problem for unloading

class Game : public Updateable
{
public:
    Game(RenderManager *renderManager, UpdateManager *updateManager,
         CollisionManager *collisionManager, World *world, Graphics *graphics,
         TextureManager *textureManager);

    ~Game();

    void update(float delta) override;
    void cleanupDestroyedShips();

private:
    void initPlayerShip();

    void addBomberShip();
    void addGunnerShip();
    void addEnemyShip(std::unique_ptr<EnemyShip> enemy);
    Vector2 getEnemySpawnLocation(int buffer);

private:
    RenderManager *renderManager;
    UpdateManager *updateManager;
    CollisionManager *collisionManager;
    TextureManager *textureManager;
    World *world;
    Graphics *graphics;
    ProjectileManager projectileManager;

    // TODO ship manager
    std::unique_ptr<PlayerShip> ship;
    std::vector<std::unique_ptr<EnemyShip>> enemyShips;
};