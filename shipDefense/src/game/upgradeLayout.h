#pragma once

#include "raylib.h"
#include <algorithm>

namespace UpgradeLayout
{
    inline Rectangle panel(Vector2 canvasSize, int index, int count)
    {
        if (count <= 0 || index < 0 || index >= count)
            return {};

        const float horizontalPadding = canvasSize.x * 0.08f;
        const float gap = std::min(canvasSize.x * 0.02f, 18.0f);
        const float panelWidth =
            (canvasSize.x - horizontalPadding * 2.0f - gap * (count - 1)) / count;
        const float panelHeight = canvasSize.y * 0.52f;
        const float totalWidth = panelWidth * count + gap * (count - 1);

        return {
            (canvasSize.x - totalWidth) / 2.0f + index * (panelWidth + gap),
            canvasSize.y * 0.30f,
            panelWidth,
            panelHeight};
    }
}