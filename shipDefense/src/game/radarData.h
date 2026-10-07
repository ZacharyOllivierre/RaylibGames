#pragma once

struct RadarData
{
    int maxRadius;
    int scanningSpeed;
    float scanningIdleTime;

    int currentRadius;
    float currentIdleTime;

    std::vector<Vector2> activeShipPositions;
};