#pragma once

#include "particle.h"

#include <vector>

class Graphics
{
public:
    Graphics(std::vector<Particle> &particles);

    void draw();

    Color getParticleColor(int typeId);

private:
private:
    std::vector<Particle> &particles;
};