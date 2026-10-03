#pragma once

#include <memory>
#include <vector>

#include "interfaces/updateable.h"

#include "ships/shipManager.h"
#include "projectiles/projectileManager.h"
#include "renderManager.h"
#include "collisionManager.h"
#include "updateManager.h"
#include "textureManager.h"
#include "graphics.h"
#include "waveSpawner.h"
#include "sceneTypes.h"

// TODO

class Game : public Updateable
{
public:
    Game(RenderManager *renderManager, UpdateManager *updateManager,
         CollisionManager *collisionManager, World *world, Graphics *graphics,
         TextureManager *textureManager);

    ~Game();

    void update(float delta) override;

    void startRound();
    void resetGame();
    GameResult getResult() const;

private:
    void initPlayerShip();

private:
    World *world;
    Graphics *graphics;
    ProjectileManager projectileManager;
    ShipManager shipManager;
    WaveSpawner waveSpawner;
    GameResult result;
};