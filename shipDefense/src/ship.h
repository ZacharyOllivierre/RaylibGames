#pragma once

#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>
#include <utility>

#include "sprite.h"
#include "world.h"

#include "interfaces/updateable.h"
#include "interfaces/physicsBody.h"
#include "interfaces/renderable.h"
#include "interfaces/collidable.h"
#include "interfaces/damageable.h"

// TODO give a cpp
struct ShipData
{
    // config data
    Vector2 startingPosition;
    Rectangle hitBox;

    // active data
    float speed = 0.0f;
    float rotation = 0.0f;
    float health = 100.0f;
};

class Ship : public Updateable,
             public PhysicsBody,
             public Renderable,
             public Collidable,
             public Damageable
{
public:
    Ship(ShipData data, World &world, Sprite sprite)
        : PhysicsBody({data.startingPosition.x,
                       data.startingPosition.y,
                       data.hitBox.width,
                       data.hitBox.height},
                      {0.99f, 0.99f}),
          data(data),
          world(world),
          sprite(std::move(sprite)),
          health(data.health) {}

    void update(float delta) override
    {
        PhysicsBody::update(delta);
        world.constrain(*this);
    }

    Rectangle getCollisionBounds() const override
    {
        return PhysicsBody::getBody();
    }

    virtual bool isCollisionImmovable() const
    {
        return false;
    }

    virtual bool isPlayerShip() const
    {
        return false;
    }

    float getHealth() const
    {
        return health;
    }

    bool isDead() const
    {
        return health <= 0.0f;
    }

    void takeDamage(const DamageEvent &damage) override
    {
        health = std::max(0.0f, health - damage.amount);
    }

    void onCollision(Collidable &other,
                     const CollisionContact &contact) override
    {
        Ship *otherShip = dynamic_cast<Ship *>(&other);
        if (otherShip == nullptr)
            return;

        if (isPlayerShip() != otherShip->isPlayerShip())
        {
            takeDamage({10.0f, DamageType::Contact, &other});
        }

        if (isCollisionImmovable())
            return;

        Vector2 velocity = getVelocity();
        Vector2 otherVelocity = otherShip->getVelocity();

        float inwardVelocity = velocity.x * contact.normal.x +
                               velocity.y * contact.normal.y;
        float otherInwardVelocity = otherVelocity.x * contact.normal.x +
                                    otherVelocity.y * contact.normal.y;
        float approachSpeed = std::max(0.0f, -inwardVelocity);
        float otherApproachSpeed = std::max(0.0f, otherInwardVelocity);
        bool otherIsImmovable = otherShip->isCollisionImmovable();

        if (!otherIsImmovable && approachSpeed > otherApproachSpeed)
            return;

        float correctionScale = otherIsImmovable
                                    ? 1.0f
                                    : (approachSpeed < otherApproachSpeed ? 1.0f : 0.5f);
        Vector2 correction = Vector2Scale(
            contact.normal,
            contact.penetration * correctionScale);
        if (inwardVelocity < 0.0f)
        {
            velocity = Vector2Subtract(
                velocity,
                Vector2Scale(contact.normal, inwardVelocity));
        }

        setPosition(Vector2Add(getPosition(), correction));
        setVelocity(velocity);
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
    float health;
};