#pragma once

#include "raylib.h"

#include "../renderManager.h"
#include "../textureManager.h"
#include "../game/gameView.h"
#include "../game/game.h"

class Graphics
{
public:
    Graphics(Vector2 windowDimensions,
             Vector2 worldDimensions,
             RenderManager *rManager,
             TextureManager *textureManager,
             Game *game);

    ~Graphics();

    Camera2D &getCamera() { return camera; };

    void update(const Scene &scene, float delta);

    void resetMainMenu() { mainMenuTimer = 0; }

private:
    void draw(const Scene &scene, const GameView &view);

    void updateTimers(const Scene &scene, float delta);
    void updateCamera(Vector2 target);

    void drawGame(const GameView &view);
    void buildGameTexture();

    // TODO function uses and names (draw | build) not consistent
    void drawMainMenu();
    void buildMainMenu();

    void drawGameOver(GameResult result);
    void buildGameOver(GameResult result);

    void drawHud(const GameView &view);
    void drawMap(const GameView &view);
    void buildHud(const GameView &view);

    void drawGrid(Rectangle &area, int spacing, Color color);
    void drawCornerRectangle(const Rectangle &area, float edgePercentage,
                             Color color, float lineThickness = 2.0f);
    Vector2 translateToMap(Vector2 worldPosition, const Rectangle &map) const;

    void DrawDoubleText(Font font, const char *text, Vector2 position, float fontSize,
                        float spacing, Color backColor, Color frontColor, Vector2 offset, float alpha = 255);

private:
    Game *game;

    Vector2 windowDimensions;
    Vector2 worldDimensions;
    Vector2 renderDimensions;

    RenderTexture2D gameTexture;
    RenderTexture2D mainMenuTexture;
    // TODO game over shouldnt be its own texture, game texture can adopt it
    RenderTexture2D gameOverTexture;
    RenderTexture2D hudTexture;

    Camera2D camera;

    RenderManager *renderManager;
    TextureManager *textureManager;

    Font gameFont;

    Shader bloom;

    // TODO change name not rlly a timer | used for alpha (fade animation)
    // TODO not great design
    float mainMenuTimer;
    float mainMenuSubTimer;
    float starRotation;
};