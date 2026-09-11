#include "particle.h"

#include "simMath.h"

Particle::Particle(const ParticleConfig &config, const Vector2 *dimensions)
{
    // Init
    data.typeId = config.typeId;
    data.position = config.startingPosition;
    data.velocity = config.startingVelocity;
    data.size = config.size;

    this->dimensions = dimensions;
}

void Particle::applyForce(Vector2 &force)
{
    data.velocity = add(data.velocity, force);
}

void Particle::applyDamping(const float damping)
{
    data.velocity = multiply(data.velocity, damping);
}

void Particle::updatePosition()
{
    data.position = add(data.position, data.velocity);

    wrapPosition();
}

void Particle::enforceMaxVelocity(const float maxSpeed)
{
    float speed = magnitude(data.velocity);

    if (speed > maxSpeed)
    {
        data.velocity = multiply(normalize(data.velocity),
                                 maxSpeed);
    }
}

// wraps particles around canvas when colliding with bounds
void Particle::wrapPosition()
{
    float width, height;
    width = (*dimensions).x;
    height = (*dimensions).y;

    if (data.position.x < 0)
        data.position.x += width;

    else if (data.position.x >= width)
        data.position.x -= width;

    if (data.position.y < 0)
        data.position.y += height;

    else if (data.position.y >= height)
        data.position.y -= height;
}