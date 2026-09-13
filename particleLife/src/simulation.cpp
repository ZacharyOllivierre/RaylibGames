#include "simulation.h"

#include "simMath.h"

// TODO Implement remove type ability : Note that gaps can not be created in type matrix
Simulation::Simulation(const SimData &data) : data(data)
{
    // fixed seg fault from mismatch attraction matrix size and num types
    this->data.attraction.resize(data.numTypes);
    for (auto &row : this->data.attraction)
    {
        row.resize(data.numTypes, 0.0f);
    }
}

// TODO 0(n^2) solution, move to GPU to run in parralell
// TODO fix effect affect mistakes
void Simulation::updateParticles()
{
    // particle being effected
    for (Particle &particle : particles)
    {
        // function doesnt include itself
        std::vector<Particle *> withinRange = getParticlesInRange(particle);
        Vector2 totalForce = {0, 0};

        // particle causing the force
        for (Particle *otherParticle : withinRange)
        {
            Vector2 affectingForce = calculateForces(particle.getDataRef(),
                                                     otherParticle->getDataRef());

            // accumulate affecting forces
            totalForce = add(totalForce, affectingForce);
        }

        // apply force / damping and update position
        particle.applyForce(totalForce);
        particle.applyDamping(data.damping);
        particle.enforceMaxVelocity(data.maxSpeed);
        particle.updatePosition();
    }
}

// TODO bug when batch adding 2+ particles outside type check does work
//  ^ adds one type for each particle outside range (all of them)
void Simulation::addParticle(const ParticleConfig config)
{
    int typeId = config.typeId;

    // if requested type outside current types add one new type
    if (typeId >= data.numTypes)
    {
        typeId = data.numTypes;

        for (auto &row : data.attraction)
        {
            row.push_back(0.0f);
        }

        data.attraction.push_back(
            std::vector<float>(data.numTypes + 1, 0.0f));

        data.numTypes++;
    }

    ParticleConfig particleConfig = config;
    particleConfig.typeId = typeId;

    particles.emplace_back(particleConfig, &data.dimensions);
}

void Simulation::removeParticle(const Vector2 position, const int radius)
{
    float radiusSquared = radius * radius;

    for (auto it = particles.begin(); it != particles.end();)
    {
        if (distanceSquared(it->getDataConst().position, position) <= radiusSquared)
        {
            it = particles.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Simulation::clear()
{
    particles.clear();
}

void Simulation::resetAttraction()
{
    for (auto &y : data.attraction)
    {
        for (size_t i = 0; i < y.size(); i++)
        {
            y[i] = 0.0f;
        }
    }
}

void Simulation::clearAttraction()
{
    data.attraction.clear();
    data.numTypes = 0;
}

void Simulation::centerAll()
{
    for (Particle &p : particles)
    {
        p.getDataRef().position = {data.dimensions.x / 2,
                                   data.dimensions.y / 2};
    }
}

void Simulation::updateParticleSize(int typeID, float size)
{
    for (Particle &p : particles)
    {
        if (typeID == p.getDataConst().typeId)
            p.getDataRef().size = size;
    }
}

// another expensive function called a shit ton
std::vector<Particle *> Simulation::getParticlesInRange(Particle &particle)
{
    std::vector<Particle *> inRange;
    float maxDistanceSquared(data.maxAttractionArea * data.maxAttractionArea);

    for (Particle &other : particles)
    {
        if (&other == &particle)
            continue;

        float distanceBetween = distanceSquared(particle.getDataConst().position,
                                                other.getDataConst().position);

        if (distanceBetween <= maxDistanceSquared)
        {
            inRange.push_back(&other);
        }
    }

    return inRange;
}

Vector2 Simulation::calculateForces(ParticleData &particle, ParticleData &other)
{
    float dist = distance(particle.position, other.position);
    Vector2 dir = direction(particle.position, other.position);

    // enforce repulsion area
    if (dist < data.innateRepulsionArea)
    {
        // apply more repulsion the closer the entities
        float forceMultiplier = 1.0f - (dist / data.innateRepulsionArea);
        return multiply(dir, data.innateRepulsion * forceMultiplier);
    }

    // matrix
    float attraction = data.attraction[particle.typeId][other.typeId];

    // enforce max attraction
    attraction = clamp(attraction, -data.maxAttraction, data.maxAttraction);

    // force falls off linearly with distance (trash)
    // float forceMultiplier = 1.0f - (dist / data.maxAttractionArea);

    // greatest force is applied at the midpoint beween the end of innate repulsion
    // and max attraction distance. Drops off on both ends of midpoint
    float midpoint = (data.innateRepulsionArea + data.maxAttractionArea) / 2.0f;

    float forceMultiplier = 1.0f - std::abs(dist - midpoint) /
                                       (data.maxAttractionArea - midpoint);

    return multiply(dir, attraction * forceMultiplier);
}