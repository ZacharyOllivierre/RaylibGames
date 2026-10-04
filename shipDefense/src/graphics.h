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

    void updateTimers(const Scene &scene, float delta);

    void setMainMenuTimer(float value) { mainMenuTimer = value; }

private:
    void drawGame(const GameView &view);
    void buildGameTexture();
    void drawMainMenu();
    void buildMainMenu();
    void drawGameOver(GameResult result);
    void buildGameOver(GameResult result);

    void drawHud(const GameView &view);

    void drawGameTestGrid();

    void DrawDoubleText(Font font, const char *text, Vector2 position, float fontSize,
                        float spacing, Color backColor, Color frontColor, Vector2 offset, float alpha = 255);

private:
    Vector2 windowDimensions;
    Vector2 worldDimensions;

    RenderTexture2D gameTexture;
    RenderTexture2D mainMenuTexture;
    RenderTexture2D gameOverTexture;

    Camera2D camera;

    RenderManager *renderManager;
    TextureManager *textureManager;

    Font gameFont;

    // TODO change name not rlly a timer | used for alpha (fade animation)
    // TODO not great design
    float mainMenuTimer;
    float mainMenuSubTimer;
    float starRotation;
};