#pragma once

#include "raylib.h"

#include <algorithm>

#include "../atlas.h"
#include "../interfaces/collidable.h"
#include "../interfaces/damageable.h"
#include "../interfaces/physicsBody.h"
#include "../interfaces/renderable.h"
#include "../interfaces/updateable.h"

class Projectile : public Updateable,
                   public PhysicsBody,
                   public Renderable,
                   public Collidable,
                   public Damageable
{
public:
    Projectile(Vector2 position, Vector2 velocity, Vector2 hitBoxSize,
               Vector2 textureSize, Atlas &atlas, Damageable &owner,
               float damage, float lifetime = 5.0f)
        : PhysicsBody({position.x, position.y, hitBoxSize.x, hitBoxSize.y},
                      {1.0f, 1.0f}),
          textureSize(textureSize),
          atlas(atlas),
          owner(owner),
          ownerTeam(owner.getTeam()),
          damage(damage),
          lifetime(lifetime)
    {
        setVelocity(velocity);
    }

    void update(float delta) override
    {
        PhysicsBody::update(delta);
        animationTime += delta;
        lifetime -= delta;
    }

    Rectangle getCollisionBounds() const override
    {
        return getBody();
    }

    void onCollision(Collidable &other,
                     const CollisionContact &) override
    {
        if (isExpired())
            return;

        Damageable *target = dynamic_cast<Damageable *>(&other);
        Damageable *projectileTarget = dynamic_cast<Projectile *>(&other);

        if (target == nullptr || target == &owner || projectileTarget != nullptr)
            return;

        if (target->getTeam() != Team::Neutral && target->getTeam() == ownerTeam)
            return;

        dealDamage(*target, damage, DamageType::Projectile);
        lifetime = 0.0f;
    }

    void takeDamage(const DamageEvent &) override
    {
    }

    void render() override
    {
        if (isExpired())
            return;

        Vector2 velocity = getVelocity();
        float rotation = atan2f(velocity.y, velocity.x) * RAD2DEG + 90.0f;

        atlas.drawCentered(
            {getBody().x + getBody().width / 2.0f,
             getBody().y + getBody().height / 2.0f},
            rotation,
            textureSize,
            atlas.getFrame(animationTime, framesPerSecond));

        // TODO debug hitbox draw
        DrawRectangleLinesEx(PhysicsBody::getBody(), 2, RED);
    }

    bool isExpired() const
    {
        return lifetime <= 0.0f;
    }

private:
    Vector2 textureSize;
    Atlas &atlas;
    Damageable &owner;
    Team ownerTeam;
    float damage;
    float lifetime;
    float animationTime = 0.0f;

    static constexpr float framesPerSecond = 12.0f;
};