#pragma once
#include "raylib.h"

#include <cmath>

inline float square(float x)
{
    return x * x;
}

inline float clamp(float value, float min, float max)
{
    return value < min ? min : (value > max ? max : value);
}

inline float magnitudeSquared(Vector2 v)
{
    return v.x * v.x + v.y * v.y;
}

inline float magnitude(Vector2 v)
{
    return std::sqrt(magnitudeSquared(v));
}

inline Vector2 normalize(Vector2 v)
{
    float magSquared = magnitudeSquared(v);

    if (magSquared == 0.0f)
        return {0.0f, 0.0f};

    float inverseMagnitude = 1.0f / std::sqrt(magSquared);

    return {
        v.x * inverseMagnitude,
        v.y * inverseMagnitude};
}

inline Vector2 displacement(Vector2 from, Vector2 to)
{
    return {
        to.x - from.x,
        to.y - from.y};
}

inline float distanceSquared(Vector2 a, Vector2 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;

    return dx * dx + dy * dy;
}

inline float distance(Vector2 a, Vector2 b)
{
    return std::sqrt(distanceSquared(a, b));
}

inline Vector2 direction(Vector2 from, Vector2 to)
{
    return normalize(displacement(from, to));
}

inline Vector2 add(Vector2 a, Vector2 b)
{
    return {
        a.x + b.x,
        a.y + b.y};
}

inline Vector2 subtract(Vector2 a, Vector2 b)
{
    return {
        a.x - b.x,
        a.y - b.y};
}

inline Vector2 multiply(Vector2 v, float scalar)
{
    return {
        v.x * scalar,
        v.y * scalar};
}

inline Vector2 divide(Vector2 v, float scalar)
{
    float inverse = 1.0f / scalar;

    return {
        v.x * inverse,
        v.y * inverse};
}