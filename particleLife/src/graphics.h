#pragma once

#include "particle.h"

#include <vector>

class Graphics
{
public:
    Graphics(std::vector<Particle> &particles);

    void draw();

private:
    Color getParticleColor(int typeId);

private:
    std::vector<Particle> &particles;
};