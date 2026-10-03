#pragma once

#include "raylib.h"

#include <functional>
#include <memory>
#include <utility>

#include "../atlas.h"
#include "../world.h"

#include "../interfaces/collidable.h"
#include "../interfaces/damageable.h"
#include "../interfaces/physicsBody.h"
#include "../interfaces/renderable.h"
#include "../interfaces/updateable.h"

class Projectile;
using ProjectileSpawner = std::function<void(std::unique_ptr<Projectile>)>;

struct ShipData
{
    Vector2 startingPosition;
    Vector2 hitBoxSize;
    Vector2 textureSize;
    float speed = 0.0f;
    float rotation = 0.0f;
    float health = 100.0f;
    float contactDamage = 1.0f;
};

class Ship : public Updateable,
             public PhysicsBody,
             public Renderable,
             public Collidable,
             public Damageable
{
public:
    Ship(ShipData data, World &world, const ShipVisuals &visuals,
         ProjectileSpawner projectileSpawner);

    void update(float delta) override;

    Rectangle getCollisionBounds() const override;

    virtual bool isPlayerShip() const;

    float getHealth() const;

    bool isDead() const;

    bool isDestructionComplete() const;

    void takeDamage(const DamageEvent &damage) override;

    void onCollision(Collidable &other,
                     const CollisionContact &contact) override;

    void render() override;

protected:
    void spawnProjectile(std::unique_ptr<Projectile> projectile);

    ShipData data;
    World &world;
    const ShipVisuals &visuals;
    ProjectileSpawner projectileSpawner;
    float animationTime = 0.0f;
    float destructionTime = 0.0f;
    bool enginesActive = false;

    static constexpr float engineFramesPerSecond = 12.0f;
    static constexpr float destructionFramesPerSecond = 12.0f;
};