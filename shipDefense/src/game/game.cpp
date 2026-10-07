#include "game.h"

Game::Game(RenderManager *renderManager, UpdateManager *updateManager,
           CollisionManager *collisionManager, World *world,
           TextureManager *textureManager)
    : world(world),
      projectileManager(*renderManager, *updateManager, *collisionManager),
      shipManager(*renderManager, *updateManager, *collisionManager, *world,
                  *textureManager, projectileManager.getSpawner()),
      waveSpawner(1.1f, 10, shipManager, *world),
      result(GameResult::InProgress)
{
    initPlayerShip();
    startRound();
}

Game::~Game()
{
    projectileManager.clear();
}

void Game::update(float delta)
{
    if (result != GameResult::InProgress)
        return;

    shipManager.cleanupDestroyedShips();
    projectileManager.cleanup();

    if (shipManager.getPlayerShip()->isDead())
    {
        result = GameResult::Lose;
        return;
    }

    if (shipManager.hasNoEnemyShips())
    {
        if (waveSpawner.hasNextWave())
        {
            startRound();
        }
        else
        {
            result = GameResult::Win;
        }
    }
}

void Game::startRound()
{
    waveSpawner.spawnNextWave();
}

void Game::resetGame()
{
    projectileManager.clear();
    shipManager.clear();
    waveSpawner.reset();
    result = GameResult::InProgress;
    initPlayerShip();
    startRound();
}

GameResult Game::getResult() const
{
    return result;
}

GameView Game::getView() const
{
    const PlayerShip *player = shipManager.getPlayerShip();
    int currentRound;
    int maxRounds;
    waveSpawner.getRoundInfo(currentRound, maxRounds);

    return {
        currentRound,
        maxRounds,
        player->getHealth(),
        player->getMaxHealth(),
        player->getPosition(),
        result};
}

void Game::initPlayerShip()
{
    Vector2 area = world->getDimensions();

    ShipData shipData{{area.x / 2, area.y / 2}, {40, 40}, {64, 64}, 600.0f, 0.0f, 100.0f, 1.0f};
    shipData.wrapAtEdges = true;
    shipManager.createShip(ShipManager::ShipType::Player, shipData);
}
