#include "game.h"

Game::Game(RenderManager *renderManager, UpdateManager *updateManager,
           CollisionManager *collisionManager, World *world, Graphics *graphics,
           TextureManager *textureManager)
    : renderManager(renderManager),
      updateManager(updateManager),
      collisionManager(collisionManager),
      textureManager(textureManager),
      world(world), graphics(graphics),
      projectileManager(*renderManager, *updateManager, *collisionManager)
{
    initPlayerShip();
    addBomberShip();
    addBomberShip();
    addBomberShip();
    addGunnerShip();
    addGunnerShip();
    addGunnerShip();
}

Game::~Game()
{
    projectileManager.clear();

    for (const auto &enemy : enemyShips)
    {
        renderManager->remove(enemy.get());
        updateManager->remove(enemy.get());
        collisionManager->remove(enemy.get());
    }
    enemyShips.clear();

    renderManager->remove(ship.get());
    updateManager->remove(ship.get());
    collisionManager->remove(ship.get());
    ship.reset();
}

void Game::update(float delta)
{
    graphics->updateCamera(ship->getPosition());

    cleanupDestroyedShips();
    projectileManager.cleanup();
}

void Game::cleanupDestroyedShips()
{
    for (auto enemy = enemyShips.begin(); enemy != enemyShips.end();)
    {
        if (!(*enemy)->isDead() || !(*enemy)->isDestructionComplete())
        {
            ++enemy;
            continue;
        }

        renderManager->remove(enemy->get());
        updateManager->remove(enemy->get());
        collisionManager->remove(enemy->get());
        enemy = enemyShips.erase(enemy);

        // TODO implement ship death ramifications
    }

    if (ship->isDead())
    {
        renderManager->remove(ship.get());
        updateManager->remove(ship.get());
        collisionManager->remove(ship.get());

        // TODO implement player death
    }
}

void Game::initPlayerShip()
{
    ShipData shipData{{100, 100}, {36, 36}, {48, 48}, 1000.0f, 0.0f, 100.0f, 1.0f};

    ship = std::make_unique<PlayerShip>(
        shipData,
        *world,
        textureManager->playerVisuals,
        textureManager->pelletAtlas,
        projectileManager.getSpawner());

    updateManager->add(ship.get());
    renderManager->add(ship.get());
    collisionManager->add(ship.get());
}

void Game::addBomberShip()
{
    Vector2 position = getEnemySpawnLocation(32);
    ShipData data{position, {64, 64}, {64, 64}, 700.0f, 0.0f, 50.0f, 0.5f};
    addEnemyShip(std::make_unique<BomberShip>(data, *world,
                                              *ship,
                                              textureManager->bomberVisuals,
                                              projectileManager.getSpawner()));
}

void Game::addGunnerShip()
{
    Vector2 position = getEnemySpawnLocation(16);
    ShipData data{position, {64, 64}, {64, 64}, 600.0f, 0.0f, 100.0f, 0.6f};
    addEnemyShip(std::make_unique<GunnerShip>(data, *world,
                                              *ship,
                                              textureManager->fighterVisuals,
                                              projectileManager.getSpawner()));
}

void Game::addEnemyShip(std::unique_ptr<EnemyShip> enemy)
{
    EnemyShip *enemyPtr = enemy.get();
    enemyShips.push_back(std::move(enemy));

    updateManager->add(enemyPtr);
    renderManager->add(enemyPtr);
    collisionManager->add(enemyPtr);
}

Vector2 Game::getEnemySpawnLocation(int buffer)
{
    // TODO cleaner with enum
    int wall = GetRandomValue(1, 4);
    Vector2 dimensions = world->getDimensions();

    // TODO creating "pos" in each if is terrible - error if pull out tho
    // top
    if (wall == 1)
    {
        Vector2 pos = {(float)GetRandomValue(buffer, dimensions.x - buffer),
                       (float)buffer};
        return pos;
    }
    // Bottom
    if (wall == 3)
    {
        Vector2 pos = {(float)GetRandomValue(buffer, dimensions.x - buffer),
                       dimensions.y - (float)buffer};
        return pos;
    }
    // right
    if (wall == 2)
    {
        Vector2 pos = {dimensions.x - buffer,
                       (float)GetRandomValue(buffer, dimensions.y - buffer)};
        return pos;
    }
    // left
    if (wall == 4)
    {
        Vector2 pos = {(float)buffer,
                       (float)GetRandomValue(buffer, dimensions.y - buffer)};
        return pos;
    }

    return {0, 0};
}