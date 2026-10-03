#include "game.h"

Game::Game(RenderManager *renderManager, UpdateManager *updateManager,
           CollisionManager *collisionManager, World *world, Graphics *graphics,
           TextureManager *textureManager)
    : world(world), graphics(graphics),
      projectileManager(*renderManager, *updateManager, *collisionManager),
      shipManager(*renderManager, *updateManager, *collisionManager, *world,
                  *textureManager, projectileManager.getSpawner()),
      waveSpawner(1.0f, 10, shipManager, *world),
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

    graphics->updateCamera(shipManager.getPlayerShip()->getPosition());

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

void Game::initPlayerShip()
{
    ShipData shipData{{100, 100}, {36, 36}, {48, 48}, 1000.0f, 0.0f, 100.0f, 1.0f};
    shipManager.createShip(ShipManager::ShipType::Player, shipData);
}
