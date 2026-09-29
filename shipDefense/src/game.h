#pragma once

#include <memory>
#include <vector>

#include "interfaces/updateable.h"

#include "playerShip.h"
#include "enemyShip.h"
#include "sprite.h"
#include "renderManager.h"
#include "updateManager.h"
#include "textureManager.h"
#include "graphics.h"

// TODO
// textures being stored in the ships themselves is a problem for unloading

class Game : public Updateable
{
public:
    Game(RenderManager *renderManager, UpdateManager *updateManager,
         World *world, Graphics *graphics, TextureManager *textureManager);

    ~Game();

    void update(float delta) override;

private:
    void initPlayerShip();

    void addEnemyShip(EnemyType type);
    Sprite getEnemyShipData(const EnemyType type, ShipData &data, FollowType &followType);
    Vector2 getEnemySpawnLocation();

private:
    RenderManager *renderManager;
    UpdateManager *updateManager;
    TextureManager *textureManager;
    World *world;
    Graphics *graphics;

    std::unique_ptr<PlayerShip> ship;
    std::vector<std::unique_ptr<EnemyShip>> enemyShips;
};