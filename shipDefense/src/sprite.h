#pragma once

#include "raylib.h"

// TODO rotation and hitbox dont line up

class Sprite
{
public:
    // constructor for sprite sheet
    Sprite(Texture2D texture, Rectangle source) : texture(texture), source(source) {}

    // constructor for single sprite
    Sprite(Texture2D texture)
        : texture(texture),
          source{0, 0,
                 static_cast<float>(texture.width),
                 static_cast<float>(texture.height)} {}

    void renderCentered(Vector2 position, float rotation, Vector2 size) const
    {
        Rectangle destination = {
            position.x,
            position.y,
            size.x,
            size.y};

        DrawTexturePro(
            texture,
            source,
            destination,
            {destination.width / 2.0f,
             destination.height / 2.0f},
            rotation,
            WHITE);
    }

    void renderHitBox(Rectangle body)
    {
        DrawRectangleLinesEx(
            body,
            2.0f,
            RED);
    }

    Vector2 getSize() const
    {
        return {
            source.width,
            source.height};
    }

private:
    Texture2D texture;
    Rectangle source;
};
