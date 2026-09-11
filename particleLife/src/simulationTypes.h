#pragma once
#include <vector>

struct SimData
{
    float innateRepulsionArea = 5;
    float innateRepulsion = -1;
    float maxAttractionArea = 50;
    float maxAttraction = 25;
    float maxSpeed = 100;
    float damping = 10;

    // canvas dimensions
    const Vector2 dimensions;

    int numTypes;
    std::vector<std::vector<float>> attraction;

    // gross glut of code to have attraction init correctly
    SimData(
        float innateRepulsionArea, float innateRepulsion, float maxAttractionArea,
        float maxAttraction, float maxSpeed,
        float damping, Vector2 dimensions, int numTypes)
        : innateRepulsionArea(innateRepulsionArea), innateRepulsion(innateRepulsion),
          maxAttractionArea(maxAttractionArea), maxAttraction(maxAttraction),
          maxSpeed(maxSpeed), damping(damping), dimensions(dimensions), numTypes(numTypes),
          attraction(numTypes, std::vector<float>(numTypes, 0.0f)) {}
};

// TODO Reduce the structs to one dont need a sep config
struct ParticleConfig
{
    int typeId;
    float size;

    Vector2 startingPosition;
    Vector2 startingVelocity;
};

struct ParticleData
{
    int typeId;

    float size;

    Vector2 position;
    Vector2 velocity;
};