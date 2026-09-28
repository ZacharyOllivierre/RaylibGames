#pragma once

#include "interfaces/updateable.h"
#include "interfaces/physicsBody.h"
#include "interfaces/renderable.h"

#include "sprite.h"
#include "world.h"

#include "raylib.h"

struct PlayerShipData
{
    // config data
    Vector2 startingPosition;
    Rectangle hitBox;

    // active data
    float speed = 0.0f;
    float rotation = 0.0f;
};

class PlayerShip : public Updateable,
                   public PhysicsBody,
                   public Renderable
{
public:
    PlayerShip(PlayerShipData &data, World &world, Sprite sprite);

    void update(float delta) override;

    void render() override;

private:
    PlayerShipData data;
    World &world;
    Sprite sprite;
};