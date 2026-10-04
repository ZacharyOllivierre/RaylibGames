#pragma once

#include "raylib.h"

#include "renderManager.h"
#include "textureManager.h"
#include "gameView.h"

class Graphics
{
public:
    Graphics(Vector2 windowDimensions,
             Vector2 worldDimensions,
             RenderManager *rManager,
             TextureManager *textureManager);

    ~Graphics();

    void draw(const Scene &scene, const GameView &view);

    Camera2D &getCamera();
    void updateCamera(Vector2 target);

private:
    void drawGame(const GameView &view);
    void buildGameTexture();
    void drawMainMenu();
    void buildMainMenu();
    void drawGameOver(GameResult result);
    void buildGameOver(GameResult result);

    void drawHud(const GameView &view);

    void drawGameTestGrid();

private:
    Vector2 windowDimensions;
    Vector2 worldDimensions;

    RenderTexture2D gameTexture;
    RenderTexture2D mainMenuTexture;
    RenderTexture2D gameOverTexture;

    Camera2D camera;

    RenderManager *renderManager;
    TextureManager *textureManager;
};