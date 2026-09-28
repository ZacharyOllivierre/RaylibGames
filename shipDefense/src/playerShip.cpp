#include "playerShip.h"
#include "raymath.h"

#include <cmath>
#include <utility>

PlayerShip::PlayerShip(PlayerShipData &data, World &world, Sprite sprite)
    : PhysicsBody({data.startingPosition.x,
                   data.startingPosition.y,
                   data.hitBox.width,
                   data.hitBox.height},
                  {0.99f, 0.99f}),
      data(data),
      world(world),
      sprite(std::move(sprite))
{
}

void PlayerShip::update(float delta)
{
    // temp ship controls
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

    PhysicsBody::update(delta);
    world.constrain(*this);
}

void PlayerShip::render()
{
    // calculate rotation
    Vector2 velocity = PhysicsBody::getVelocity();

    // update rotation if velocity "significant" | avoids snapping back at 0
    if (Vector2Length(velocity) > 0.01f)
    {
        data.rotation = atan2f(velocity.y, velocity.x) * RAD2DEG + 90.0f;
    }

    Rectangle body = PhysicsBody::getBody();

    // render ship in center of body
    Vector2 center = {
        body.x + body.width / 2.0f,
        body.y + body.height / 2.0f};

    sprite.renderCentered(center, data.rotation,
                          {data.hitBox.x, data.hitBox.y});

    sprite.renderHitBox(body);
}