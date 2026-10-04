#include "ship.h"

#include "../projectiles/projectile.h"

#include "raymath.h"

#include <algorithm>
#include <cmath>

Ship::Ship(ShipData data, World &world, const ShipVisuals &visuals,
           ProjectileSpawner projectileSpawner)
    : PhysicsBody({data.startingPosition.x,
                   data.startingPosition.y,
                   data.hitBoxSize.x,
                   data.hitBoxSize.y},
                  {0.991f, 0.991f}),
      data(data),
      world(world),
      visuals(visuals),
      projectileSpawner(std::move(projectileSpawner))
{
    if (this->data.maxHealth <= 0.0f)
        this->data.maxHealth = this->data.health;
}

void Ship::spawnProjectile(std::unique_ptr<Projectile> projectile)
{
    if (projectileSpawner)
        projectileSpawner(std::move(projectile));
}

void Ship::update(float delta)
{
    enginesActive = Vector2Length(getAcceleration()) > 0.0f;
    animationTime += delta;

    if (isDead())
        destructionTime += delta;

    if (isDead())
        return;

    PhysicsBody::update(delta);
    world.constrain(*this, data.wrapAtEdges);
}

Rectangle Ship::getCollisionBounds() const
{
    return PhysicsBody::getBody();
}

bool Ship::isPlayerShip() const
{
    return false;
}

float Ship::getHealth() const
{
    return data.health;
}

float Ship::getMaxHealth() const
{
    return data.maxHealth;
}

bool Ship::isDead() const
{
    return data.health <= 0.0f;
}

bool Ship::isDestructionComplete() const
{
    return destructionTime >= visuals.destruction.getAnimationDuration(
                                  destructionFramesPerSecond);
}

void Ship::takeDamage(const DamageEvent &damage)
{
    if (isDead())
        return;

    data.health = std::max(0.0f, data.health - damage.amount);
    if (isDead())
        destructionTime = 0.0f;
}

void Ship::onCollision(Collidable &other,
                       const CollisionContact &contact)
{
    Ship *otherShip = dynamic_cast<Ship *>(&other);
    if (otherShip == nullptr)
        return;

    if (isPlayerShip() != otherShip->isPlayerShip())
    {
        dealDamage(*otherShip, data.contactDamage, DamageType::Contact);
    }

    addForce(Vector2Scale(contact.normal, contact.penetration) * 50);
}

void Ship::render()
{
    Vector2 velocity = PhysicsBody::getVelocity();

    if (Vector2Length(velocity) > 0.01f)
    {
        data.rotation = atan2f(velocity.y, velocity.x) * RAD2DEG + 90.0f;
    }

    Rectangle body = PhysicsBody::getBody();
    Vector2 center = {
        body.x + body.width / 2.0f,
        body.y + body.height / 2.0f};

    if (isDead())
    {
        visuals.destruction.drawCentered(
            center, data.rotation, data.textureSize,
            visuals.destruction.getFrame(destructionTime,
                                         destructionFramesPerSecond,
                                         false));
        return;
    }

    DrawTexturePro(
        visuals.base,
        {0, 0, static_cast<float>(visuals.base.width),
         static_cast<float>(visuals.base.height)},
        {center.x, center.y, data.textureSize.x, data.textureSize.y},
        {data.textureSize.x / 2.0f, data.textureSize.y / 2.0f},
        data.rotation,
        WHITE);

    if (enginesActive)
    {
        visuals.engines.drawCentered(
            center, data.rotation, data.textureSize,
            visuals.engines.getFrame(animationTime, engineFramesPerSecond));
    }

    // TODO debug hitbox draw
    // DrawRectangleLinesEx(body, 2.0f, RED);
}