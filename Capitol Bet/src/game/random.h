#pragma once

#include <cstdint>
#include <random>

class RandomSource
{
public:
    explicit RandomSource(std::uint32_t seed) : generator(seed) {}

    float value()
    {
        return distribution(generator);
    }

    bool chance(float probability)
    {
        return probability > 0.0f &&
               (probability >= 1.0f || unitDistribution(generator) < probability);
    }

    int integer(int minimum, int maximum)
    {
        return std::uniform_int_distribution<int>(minimum, maximum)(generator);
    }

private:
    std::mt19937 generator;
    std::uniform_real_distribution<float> distribution{-1.0f, 1.0f};
    std::uniform_real_distribution<float> unitDistribution{0.0f, 1.0f};
};
