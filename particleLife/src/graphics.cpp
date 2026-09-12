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

Color Graphics::getParticleColor(int typeId)
{
    // float hue = fmodf(typeId * 137.5f, 360.0f);
    // Color color = ColorFromHSV(hue, 0.8f, 1.0f);
    switch (typeId)
    {
    case static_cast<int>(SimColors::Red):
        return RED;
    case static_cast<int>(SimColors::Green):
        return GREEN;
    case static_cast<int>(SimColors::Blue):
        return BLUE;
    case static_cast<int>(SimColors::Purple):
        return PURPLE;
    case static_cast<int>(SimColors::Yellow):
        return YELLOW;
    case static_cast<int>(SimColors::Orange):
        return ORANGE;
    case static_cast<int>(SimColors::Pink):
        return PINK;
    case static_cast<int>(SimColors::White):
        return WHITE;
    case static_cast<int>(SimColors::Lime):
        return LIME;
    case static_cast<int>(SimColors::Skyblue):
        return SKYBLUE;
    case static_cast<int>(SimColors::Violet):
        return VIOLET;
    case static_cast<int>(SimColors::Brown):
        return BROWN;
    default:
        return {0, 0, 0, 255};
    }
}