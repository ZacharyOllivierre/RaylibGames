#pragma once
#include "raylib.h"

#include "particle.h"
#include "simulationTypes.h"

#include <vector>

// simulation stores the particles and applies calculations to them

class Simulation
{
public:
    Simulation(const SimData &data);

    void updateParticles();

    void addParticle(ParticleConfig config);
    void removeParticle(const Vector2 position, const int radius);

    void clear();
    void resetAttraction();

    SimData &getData() { return data; }
    std::vector<Particle> &getParticles() { return particles; }

    void randomizePositions();
    void updateParticleSize(int typeID, float size);

private:
    // cpu implementation
    std::vector<Particle *> getParticlesInRange(Particle &particle);
    Vector2 calculateForces(ParticleData &particle, ParticleData &other);

private:
    SimData data;
    std::vector<Particle> particles;
};