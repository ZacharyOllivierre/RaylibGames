#pragma once

#include "raylib.h"

#include <algorithm>

class Atlas
{
public:
    Atlas(Texture2D &texture, Vector2 frameSize)
        : texture(texture), frameSize(frameSize),
          frameCount(std::max(1, static_cast<int>(texture.width / frameSize.x)))
    {
    }

    int getFrameCount() const
    {
        return frameCount;
    }

    float getAnimationDuration(float framesPerSecond) const
    {
        return frameCount / framesPerSecond;
    }

    int getFrame(float elapsed, float framesPerSecond, bool loop = true) const
    {
        int frame = static_cast<int>(elapsed * framesPerSecond);
        if (loop)
            return frame % frameCount;

        return std::min(frame, frameCount - 1);
    }

    void drawCentered(Vector2 position, float rotation, Vector2 size,
                      int frame) const
    {
        frame = std::clamp(frame, 0, frameCount - 1);

        Rectangle source = {
            frameSize.x * frame,
            0.0f,
            frameSize.x,
            frameSize.y};
        Rectangle destination = {
            position.x,
            position.y,
            size.x,
            size.y};

        DrawTexturePro(texture, source, destination,
                       {destination.width / 2.0f,
                        destination.height / 2.0f},
                       rotation, WHITE);
    }

private:
    Texture2D &texture;
    Vector2 frameSize;
    int frameCount;
};

struct ShipVisuals
{
    Texture2D &base;
    Atlas &engines;
    Atlas &destruction;
};