#include "graphics.h"

#include "raymath.h"
#include <algorithm>

// sets game viewport as 75% and ui as 25%
Graphics::Graphics(Vector2 windowDimensions, Vector2 worldDimensions,
                   RenderManager *rManager, TextureManager *textureManager)
    : windowDimensions(windowDimensions),
      worldDimensions(worldDimensions),
      renderManager(rManager),
      textureManager(textureManager)
{
    gameTexture = LoadRenderTexture(windowDimensions.x, windowDimensions.y);

    // init camera
    camera = {
        {windowDimensions.x / 2.0f, windowDimensions.y / 2.0f},
        {windowDimensions.x / 2.0f, windowDimensions.y / 2.0f},
        0.0f,
        1.0f};
};

Graphics::~Graphics()
{
    UnloadRenderTexture(gameTexture);
}

void Graphics::drawGame()
{
    buildGameTexture();

    BeginDrawing();

    DrawTextureRec(gameTexture.texture,
                   {0, 0, windowDimensions.x, -windowDimensions.y},
                   {0, 0},
                   WHITE);

    DrawFPS(10, 10);

    EndDrawing();
}

Camera2D &Graphics::getCamera()
{
    return camera;
}

void Graphics::updateCamera(Vector2 target)
{
    float halfWidth = windowDimensions.x / 2.0f;
    float halfHeight = windowDimensions.y / 2.0f;

    // if the world is smaller than the viewport keep the camera centered on the world
    if (worldDimensions.x <= windowDimensions.x)
    {
        camera.target.x = worldDimensions.x / 2.0f;
    }
    else
    {
        camera.target.x = Clamp(
            target.x,
            halfWidth,
            worldDimensions.x - halfWidth);
    }

    if (worldDimensions.y <= windowDimensions.y)
    {
        camera.target.y = worldDimensions.y / 2.0f;
    }
    else
    {
        camera.target.y = Clamp(
            target.y,
            halfHeight,
            worldDimensions.y - halfHeight);
    }
}

void Graphics::buildGameTexture()
{
    BeginTextureMode(gameTexture);

    ClearBackground(BLACK);

    BeginMode2D(camera);

    Texture2D *stars = &textureManager->starBackground;
    DrawTexturePro(*stars,
                   {0, 0, (float)stars->width, (float)stars->height},
                   {0, 0, worldDimensions.x, worldDimensions.y},
                   {0, 0},
                   0.0f,
                   WHITE);

    // drawGameTestGrid();
    renderManager->render();

    EndMode2D();

    EndTextureMode();
}

void Graphics::drawGameTestGrid()
{
    for (int x = 0; x <= worldDimensions.x; x += 100)
    {
        DrawLine(x, 0, x, worldDimensions.y,
                 WHITE);
    }

    for (int y = 0; y <= worldDimensions.y; y += 100)
    {
        DrawLine(0, y, worldDimensions.x, y,
                 WHITE);
    }
}