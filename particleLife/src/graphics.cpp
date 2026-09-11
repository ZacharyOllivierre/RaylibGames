#include "graphics.h"

#include "raylib.h"

Graphics::Graphics(std::vector<Particle> &particles)
    : particles(particles)
{
}

void Graphics::draw()
{
    // Draw particles
    for (const Particle &particle : particles)
    {
        const ParticleData &data = particle.getDataConst();

        DrawCircleV(
            data.position,
            data.size,
            getParticleColor(particle.getDataConst().typeId));
    }
}

// TODO update function (ripped from the internet)
Color Graphics::getParticleColor(int typeId)
{
    float hue = fmodf(typeId * 137.5f, 360.0f);

    Color color = ColorFromHSV(hue, 0.8f, 1.0f);
    return color;
}