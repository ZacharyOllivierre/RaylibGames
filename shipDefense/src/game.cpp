#include "game.h"

Game::Game(RenderManager *renderManager, UpdateManager *updateManager,
           World *world, Graphics *graphics)
    : renderManager(renderManager),
      updateManager(updateManager),
      world(world), graphics(graphics)
{
    initPlayerShip();
}

void Game::update(float delta)
{
    graphics->updateCamera(ship->getPosition());
}

void Game::initPlayerShip()
{
    PlayerShipData shipData{
        {100, 100},
        {100, 100, 64, 64},
        500};

    Texture2D shipTexture = LoadTexture("assets/textures/mainShipFullHealth.png");

    Sprite shipSprite(shipTexture);

    ship = std::make_unique<PlayerShip>(
        shipData,
        *world,
        std::move(shipSprite));

    updateManager->add(ship.get());
    renderManager->add(ship.get());
}

void Game::reset()
{
    if (ship)
    {
        updateManager->remove(ship.get());
        renderManager->remove(ship.get());
    }

    initPlayerShip();
}
