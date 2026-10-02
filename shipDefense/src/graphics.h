#pragma once

#include "raylib.h"

#include "renderManager.h"

class Graphics
{
public:
    Graphics(Vector2 windowDimensions,
             Vector2 worldDimensions,
             RenderManager *rManager);

    ~Graphics();

    void drawGame();

    Camera2D &getCamera();
    void updateCamera(Vector2 target);

private:
    void buildGameTexture();

    void drawGameTestGrid();

private:
    Vector2 windowDimensions;
    Vector2 worldDimensions;

    RenderTexture2D gameTexture;

    Camera2D camera;

    RenderManager *renderManager;
};