#include "game.h"

#include <stdexcept>

Game::Game(RenderManager *renderManager, UpdateManager *updateManager,
           World *world, Graphics *graphics, TextureManager *textureManager)
    : renderManager(renderManager),
      updateManager(updateManager),
      textureManager(textureManager),
      world(world), graphics(graphics)
{
    initPlayerShip();
    addEnemyShip(EnemyType::Bomber);
    addEnemyShip(EnemyType::Bomber);
    addEnemyShip(EnemyType::Bomber);
    addEnemyShip(EnemyType::Bomber);
}

Game::~Game()
{
    for (const auto &enemy : enemyShips)
    {
        renderManager->remove(enemy.get());
        updateManager->remove(enemy.get());
    }
    enemyShips.clear();

    renderManager->remove(ship.get());
    updateManager->remove(ship.get());
    ship.reset();
}

void Game::update(float delta)
{
    graphics->updateCamera(ship->getPosition());
}

void Game::initPlayerShip()
{
    ShipData shipData{
        {100, 100},
        {100, 100, 64, 64},
        500.0f,
        0.0f};

    Sprite shipSprite(textureManager->playerShip);

    ship = std::make_unique<PlayerShip>(
        shipData,
        *world,
        std::move(shipSprite));

    updateManager->add(ship.get());
    renderManager->add(ship.get());
}

void Game::addEnemyShip(EnemyType type)
{
    ShipData data;
    FollowType followType;
    Sprite sprite = getEnemyShipData(type, data, followType);

    auto enemy = std::make_unique<EnemyShip>(
        data,
        *world,
        std::move(sprite),
        followType,
        *ship, type);

    EnemyShip *enemyPtr = enemy.get();
    enemyShips.push_back(std::move(enemy));

    updateManager->add(enemyPtr);
    renderManager->add(enemyPtr);
}

Sprite Game::getEnemyShipData(const EnemyType type, ShipData &data,
                              FollowType &followType)
{
    switch (type)
    {
    case EnemyType::Bomber:
        data.startingPosition = getEnemySpawnLocation(32);
        data.hitBox = {data.startingPosition.x, data.startingPosition.y, 64, 64};
        data.speed = 300.0f;
        data.rotation = 0.0f;

        followType = FollowType::TouchRandom;

        return Sprite(textureManager->bomberShip);

    default:
        throw std::invalid_argument("Unsupported enemy type");
    }
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