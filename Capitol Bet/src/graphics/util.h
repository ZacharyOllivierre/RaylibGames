#pragma once

#include "raylib.h"
#include <string>

namespace GraphicsUtil
{
    inline void drawGlowText(
        const std::string &text,
        Vector2 position,
        int fontSize,
        Color color,
        Color glowColor)
    {
        DrawText(
            text.c_str(),
            static_cast<int>(position.x - 2.0f),
            static_cast<int>(position.y),
            fontSize,
            glowColor);
        DrawText(
            text.c_str(),
            static_cast<int>(position.x + 2.0f),
            static_cast<int>(position.y),
            fontSize,
            glowColor);
        DrawText(
            text.c_str(),
            static_cast<int>(position.x),
            static_cast<int>(position.y - 2.0f),
            fontSize,
            glowColor);
        DrawText(
            text.c_str(),
            static_cast<int>(position.x),
            static_cast<int>(position.y + 2.0f),
            fontSize,
            glowColor);
        DrawText(
            text.c_str(),
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            fontSize,
            color);
    }
}
