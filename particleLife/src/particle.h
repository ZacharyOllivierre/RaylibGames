#pragma once
#include "raylib.h"

#include "simulationTypes.h"

// maybe turn into struct not a whole use for class
class Particle
{
public:
    // Type id meant to represent color class
    Particle(const ParticleConfig &config, const Vector2 *dimensions);

    void applyForce(Vector2 &force);
    void applyDamping(const float damping);
    void updatePosition();
    void enforceMaxVelocity(const float maxSpeed);
    void wrapPosition();

    ParticleData &getDataRef() { return data; }
    const ParticleData &getDataConst() const { return data; }

private:
    ParticleData data;

    // world bounds
    const Vector2 *dimensions;
};