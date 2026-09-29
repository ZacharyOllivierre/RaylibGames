#pragma once

#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <utility>

#include "sprite.h"
#include "world.h"

#include "interfaces/updateable.h"
#include "interfaces/physicsBody.h"
#include "interfaces/renderable.h"

struct ShipData
{
    // config data
    Vector2 startingPosition;
    Rectangle hitBox;

    // active data
    float speed = 0.0f;
    float rotation = 0.0f;
};

class Ship : public Updateable,
             public PhysicsBody,
             public Renderable
{
public:
    Ship(ShipData data, World &world, Sprite sprite)
        : PhysicsBody({data.startingPosition.x,
                       data.startingPosition.y,
                       data.hitBox.width,
                       data.hitBox.height},
                      {0.99f, 0.99f}),
          data(std::move(data)),
          world(world),
          sprite(std::move(sprite)) {}

    void update(float delta) override
    {
        PhysicsBody::update(delta);
        world.constrain(*this);
    }

    void render() override
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
                              {body.width, body.height});

        sprite.renderHitBox(body);
    }

protected:
    ShipData data;
    World &world;
    Sprite sprite;
};