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
#include "waveSpawner.h"
#include "gameView.h"

// TODO

class Game : public Updateable
{
public:
    Game(RenderManager *renderManager, UpdateManager *updateManager,
         CollisionManager *collisionManager, World *world,
         TextureManager *textureManager);

    ~Game();

    void update(float delta) override;

    void startRound();
    void resetGame();
    GameResult getResult() const;
    GameView getView() const;

private:
    void initPlayerShip();

private:
    World *world;
    ProjectileManager projectileManager;
    ShipManager shipManager;
    WaveSpawner waveSpawner;
    GameResult result;
};