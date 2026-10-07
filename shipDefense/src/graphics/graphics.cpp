#include "graphics.h"

#include "raymath.h"
#include <algorithm>

Graphics::Graphics(Vector2 windowDimensions, Vector2 worldDimensions,
                   RenderManager *rManager, TextureManager *textureManager)
    : windowDimensions(windowDimensions),
      worldDimensions(worldDimensions),
      renderDimensions({854, 480}),
      renderManager(rManager),
      textureManager(textureManager),
      bloom(LoadShader(nullptr, "src/shader/bloom.fs")),
      mainMenuTimer(0),
      mainMenuSubTimer(0),
      starRotation(0)
{
    gameTexture = LoadRenderTexture(renderDimensions.x, renderDimensions.y);
    mainMenuTexture = LoadRenderTexture(windowDimensions.x, windowDimensions.y);
    gameOverTexture = LoadRenderTexture(windowDimensions.x, windowDimensions.y);
    hudTexture = LoadRenderTexture(windowDimensions.x, windowDimensions.y);

    // init camera
    camera = {
        {renderDimensions.x / 2.0f, renderDimensions.y / 2.0f},
        {renderDimensions.x / 2.0f, renderDimensions.y / 2.0f},
        0.0f,
        0.8f};

    // font
    gameFont = LoadFontEx(
        "assets/fonts/Doom2016.ttf",
        450,
        nullptr,
        0);

    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(gameTexture.texture, TEXTURE_FILTER_POINT);
};

Graphics::~Graphics()
{
    UnloadRenderTexture(gameTexture);
    UnloadRenderTexture(mainMenuTexture);
    UnloadRenderTexture(gameOverTexture);
    UnloadRenderTexture(hudTexture);

    UnloadFont(gameFont);

    UnloadShader(bloom);
}

void Graphics::draw(const Scene &scene, const GameView &view)
{
    switch (scene)
    {
    case Scene::MainMenu:
        drawMainMenu();
        break;
    case Scene::Game:
        drawGame(view);
        break;
    case Scene::GameOver:
        drawGameOver(view.result);
        break;
    }
}

Camera2D &Graphics::getCamera()
{
    return camera;
}

void Graphics::updateCamera(Vector2 target)
{
    float halfWidth = renderDimensions.x / (2.0f * camera.zoom);
    float halfHeight = renderDimensions.y / (2.0f * camera.zoom);

    // if the world is smaller than the viewport keep the camera centered on the world
    if (worldDimensions.x <= renderDimensions.x)
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

    if (worldDimensions.y <= renderDimensions.y)
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

void Graphics::updateTimers(const Scene &scene, float delta)
{
    if (scene == Scene::MainMenu)
    {
        if (mainMenuTimer <= 255)
            mainMenuTimer += delta / 2.5;

        if (mainMenuSubTimer <= 6)
            mainMenuSubTimer += delta * 4;
        else
            mainMenuSubTimer = 0;
    }
    else if (scene == Scene::Game)
    {
        if (starRotation >= 360)
        {
            starRotation = 0;
        }
        else
        {
            starRotation += delta * 4;
        }
    }
    else if (scene == Scene::GameOver)
    {
    }
}

void Graphics::drawGame(const GameView &view)
{
    updateCamera(view.cameraTarget);

    // build
    buildGameTexture();
    buildHud(view);

    BeginDrawing();
    ClearBackground(BLACK);

    BeginShaderMode(bloom);

    DrawTexturePro(
        gameTexture.texture,
        {0, 0,
         (float)gameTexture.texture.width, -(float)gameTexture.texture.height},
        {0, 0, windowDimensions.x, windowDimensions.y},
        {0, 0}, 0.0f, WHITE);

    EndShaderMode();

    DrawTextureRec(hudTexture.texture,
                   {0, 0, windowDimensions.x, -windowDimensions.y},
                   {0, 0},
                   WHITE);

    // DrawFPS(10, 10);
    EndDrawing();
}

void Graphics::buildGameTexture()
{
    BeginTextureMode(gameTexture);

    ClearBackground(BLACK);

    BeginMode2D(camera);

    // Background
    Texture2D *stars = &textureManager->starBackground;
    DrawTexturePro(*stars,
                   {0, 0, (float)stars->width, (float)stars->height},
                   {0, 0, worldDimensions.x, worldDimensions.y},
                   {0, 0},
                   0.0f,
                   WHITE);

    Texture2D *starsEmpty = &textureManager->starEmpty;
    Vector2 center = {
        worldDimensions.x / 2.0f,
        worldDimensions.y / 2.0f};

    // animated background inverted
    DrawTexturePro(
        *starsEmpty,
        {0, 0, -(float)starsEmpty->width, -(float)starsEmpty->height},
        {center.x, center.y, worldDimensions.x, worldDimensions.y},
        {worldDimensions.x / 2.0f, worldDimensions.y / 2.0f},
        starRotation,
        WHITE);

    // Objects
    // drawGameTestGrid();
    renderManager->render();

    EndMode2D();

    EndTextureMode();
}

void Graphics::drawMainMenu()
{
    buildMainMenu();

    BeginDrawing();

    DrawTextureRec(mainMenuTexture.texture,
                   {0, 0, windowDimensions.x, -windowDimensions.y},
                   {0, 0},
                   WHITE);
    EndDrawing();
}

void Graphics::buildMainMenu()
{
    BeginTextureMode(mainMenuTexture);
    ClearBackground(BLACK);

    const char *titleText = "ShipS AnD StufF";
    const int titleFontSize = 450;
    const float titleSpacing = 3.0f;

    Vector2 titleSize = MeasureTextEx(
        gameFont, titleText,
        titleFontSize, titleSpacing);

    Vector2 titlePosition = {
        (windowDimensions.x - titleSize.x) / 2.0f,
        (float)(windowDimensions.y * 0.10)};

    DrawDoubleText(
        gameFont,
        titleText,
        titlePosition,
        titleFontSize,
        titleSpacing,
        RED,
        WHITE,
        {5, 5}, mainMenuTimer);

    // flashing "continue", 6 is timer max
    if (mainMenuSubTimer <= 4)
    {
        char subText[] = "press space to continue";
        int subFontSize = 180;
        int spacing = 1;

        DrawDoubleText(
            gameFont,
            subText,
            {windowDimensions.x / 2 -
                 MeasureTextEx(gameFont, subText, subFontSize, spacing).x / 2,
             windowDimensions.y * 0.75f - subFontSize},
            subFontSize,
            spacing,
            RED,
            WHITE,
            {3, 3});
    }

    EndTextureMode();
}

void Graphics::drawGameOver(GameResult result)
{
    buildGameOver(result);

    BeginDrawing();

    DrawTextureRec(gameOverTexture.texture,
                   {0, 0, windowDimensions.x, -windowDimensions.y},
                   {0, 0},
                   WHITE);

    EndDrawing();
}

void Graphics::buildGameOver(GameResult result)
{
    BeginTextureMode(gameOverTexture);
    ClearBackground(BLACK);

    const char *text = result == GameResult::Win ? "WiN" : "LosT";
    const int fontSize = 240;
    const float spacing = 3.0f;

    Vector2 textSize = MeasureTextEx(
        gameFont,
        text,
        fontSize,
        spacing);

    DrawDoubleText(
        gameFont, text,
        {(windowDimensions.x - textSize.x) / 2.0f,
         windowDimensions.y / 2.0f - fontSize},
        fontSize, spacing,
        result == GameResult::Win ? GREEN : RED,
        WHITE,
        {5, 5});

    const char *subText = "press space to restart";
    const int subFontSize = 120;
    const float subSpacing = 1.0f;

    Vector2 subTextSize = MeasureTextEx(
        gameFont,
        subText,
        subFontSize,
        subSpacing);

    DrawDoubleText(
        gameFont, subText,
        {(windowDimensions.x - subTextSize.x) / 2.0f,
         windowDimensions.y * 0.75f - subFontSize},
        subFontSize, subSpacing,
        RED,
        WHITE,
        {3, 3});

    EndTextureMode();
}

void Graphics::drawHud(const GameView &view)
{
    const int fontSize = 180;
    const int barWidth = 400;
    const int barHeight = 30;
    const float healthRatio = view.playerMaxHealth > 0.0f
                                  ? view.playerHealth / view.playerMaxHealth
                                  : 0.0f;

    int healthEdgeBuffer = 30;
    int hudUiY = 20;
    int healthBarStartX = windowDimensions.x - barWidth - healthEdgeBuffer;

    // round
    DrawDoubleText(
        gameFont,
        TextFormat("RounD %d / %d", view.currentRound, view.maxRounds),
        {static_cast<float>(healthEdgeBuffer), static_cast<float>(hudUiY)},
        fontSize,
        1.0f,
        RED,
        WHITE,
        {2, 2});

    // TODO fix | currently: hudUi to reference text center with magic number offset
    // healthbar
    hudUiY = hudUiY + (fontSize - barHeight) / 2 - 5;
    DrawRectangle(healthBarStartX, hudUiY, barWidth, barHeight, DARKGRAY);

    DrawRectangle(healthBarStartX, hudUiY,
                  static_cast<int>(barWidth * Clamp(healthRatio, 0.0f, 1.0f)),
                  barHeight, RED);

    DrawRectangleLines(healthBarStartX, hudUiY, barWidth, barHeight, WHITE);

    // TODO test
    // map
    Rectangle map = {
        windowDimensions.x - 300,
        windowDimensions.y - 150,
        300.0f,
        150.0f};

    Color gridColor = Color{255, 255, 255, 200};

    // draw background, grid, and outline
    DrawRectangleRec(map, Color{0, 0, 0, 150});
    DrawRectangleLinesEx(map, 2, gridColor);
    drawGrid(map, map.height / 5, gridColor);

    // TODO bug even positioning around player not correct
    // draw player rec
    Vector2 playerPos = view.cameraTarget;
    int sizeDivisor = 5;
    float width = map.width / sizeDivisor;
    float height = map.height / sizeDivisor;

    // use smaller of would be sizes for player rec
    float size = std::min(width, height);

    // find position relative to map rectangle
    Vector2 positionPercent = {
        playerPos.x / worldDimensions.x,
        playerPos.y / worldDimensions.y,
    };

    DrawRectangleLinesEx(
        {map.x + map.width * positionPercent.x,
         map.y + map.height * positionPercent.y,
         size, size},
        2, gridColor);
}

void Graphics::buildHud(const GameView &view)
{
    BeginTextureMode(hudTexture);
    ClearBackground(BLANK);
    drawHud(view);
    EndTextureMode();
}

void Graphics::drawGrid(Rectangle &area, int spacing, Color color)
{
    for (int x = 0; x <= area.width; x += spacing)
    {
        DrawLine(area.x + x, area.y,
                 area.x + x, area.y + area.height,
                 color);
    }

    for (int y = 0; y <= area.height; y += spacing)
    {
        DrawLine(area.x, area.y + y,
                 area.x + area.width, area.y + y,
                 color);
    }
}

void Graphics::DrawDoubleText(Font font, const char *text, Vector2 position, float fontSize,
                              float spacing, Color backColor, Color frontColor, Vector2 offset, float alpha)
{
    backColor = Fade(backColor, alpha);
    frontColor = Fade(frontColor, alpha);

    DrawTextEx(
        font, text,
        {position.x + offset.x, position.y + offset.y},
        fontSize, spacing, backColor);

    DrawTextEx(
        font, text, position, fontSize,
        spacing, frontColor);
}