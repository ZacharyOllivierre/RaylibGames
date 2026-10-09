#pragma once

#include <memory>
#include <vector>

#include "../interfaces/updateable.h"

#include "../ships/shipManager.h"
#include "../projectiles/projectileManager.h"
#include "../renderManager.h"
#include "../collisionManager.h"
#include "../updateManager.h"
#include "../textureManager.h"
#include "waveSpawner.h"
#include "gameView.h"
#include "radar.h"
#include "upgradeTypes.h"
#include "upgradeManager.h"
#include "../audioManager.h"

// TODO

class Game : public Updateable
{
public:
    Game(RenderManager *renderManager, UpdateManager *updateManager,
         CollisionManager *collisionManager, World *world,
         TextureManager *textureManager, AudioManager *audioManager);

    ~Game();

    void update(float delta) override;

    void startRound();
    void resetGame();
    GameResult getResult() const { return result; }
    GameView getView() const;

    const RadarData &getRadarData() { return radar.getData(); }
    std::vector<UpgradeData> getUpgradeList() const { return upgradeManager.getUpgradeData(); }

private:
    void initPlayerShip();

    void chooseUpgrade(Vector2 mousePosition, Vector2 viewportSize);

    void fillRadar(float delta);

private:
    World *world;
    AudioManager *audioManager;
    ProjectileManager projectileManager;
    ShipManager shipManager;
    WaveSpawner waveSpawner;
    GameResult result;
    Radar radar;

    UpgradeManager upgradeManager;
    bool upgradeWindowActive;
};