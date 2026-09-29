#include "playerShip.h"

PlayerShip::PlayerShip(const ShipData &data, World &world, Sprite sprite)
    : Ship(data, world, sprite)
{
}

void PlayerShip::update(float delta)
{
    shipInputControls();

    Ship::update(delta);
}

void PlayerShip::shipInputControls()
{
    // basic movement
    if (IsKeyDown(KEY_W))
    {
        addForce({0, -data.speed});
    }
    if (IsKeyDown(KEY_S))
    {
        addForce({0, data.speed});
    }
    if (IsKeyDown(KEY_A))
    {
        addForce({-data.speed, 0});
    }
    if (IsKeyDown(KEY_D))
    {
        addForce({data.speed, 0});
    }
}