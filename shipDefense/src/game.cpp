#include "game.h"

#include <stdexcept>

Game::Game(RenderManager *renderManager, UpdateManager *updateManager,
           World *world, Graphics *graphics, TextureManager *textureManager)
    : renderManager(renderManager),
      updateManager(updateManager),
      world(world), graphics(graphics),
      textureManager(textureManager)
{
    initPlayerShip();
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
        *ship);

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
        data.startingPosition = getEnemySpawnLocation();
        data.hitBox = {data.startingPosition.x, data.startingPosition.y, 64, 64};
        data.speed = 400.0f;
        data.rotation = 0.0f;

        followType = FollowType::Touch;

        return Sprite(textureManager->enemyShip);

    default:
        throw std::invalid_argument("Unsupported enemy type");
    }
}

Vector2 Game::getEnemySpawnLocation()
{
    return {50, 50};
}