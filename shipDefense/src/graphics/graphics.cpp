#include "graphics.h"

#include "raymath.h"
#include <algorithm>

Graphics::Graphics(Vector2 windowDimensions, Vector2 worldDimensions,
                   RenderManager *rManager, TextureManager *textureManager, Game *game)
    : game(game),
      windowDimensions(windowDimensions),
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

void Graphics::update(const Scene &scene, float delta)
{
    const GameView &view = game->getView();

    updateTimers(scene, delta);
    updateCamera(view.cameraTarget);
    draw(scene, view);
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

void Graphics::drawGame(const GameView &view)
{
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

    drawMap(view);
}

void Graphics::drawMap(const GameView &view)
{
    Rectangle map = {
        windowDimensions.x - 400,
        windowDimensions.y - 200,
        400.0f,
        200.0f};

    Color gridColor = Color{100, 220, 150, 190};

    // draw background and grid
    DrawRectangleRec(map, Color{0, 0, 0, 150});
    drawGrid(map, map.height / 5, gridColor);

    const Vector2 playerPosition = translateToMap(view.cameraTarget, map);
    const RadarData &radar = game->getRadarData();
    const float mapScale = map.width / worldDimensions.x;

    BeginScissorMode(static_cast<int>(map.x), static_cast<int>(map.y),
                     static_cast<int>(map.width), static_cast<int>(map.height));

    DrawCircleV(playerPosition,
                radar.currentRadius * mapScale,
                Fade(GREEN, 0.4f));
    DrawCircleLinesV(playerPosition,
                     radar.currentRadius * mapScale,
                     Fade(GREEN, 0.9f));

    for (const Vector2 enemyPosition : radar.activeShipPositions)
    {
        const Vector2 mapPosition = translateToMap(enemyPosition, map);
        DrawCircleV(mapPosition, 3.0f, RED);
        drawCornerRectangle({mapPosition.x - 8.0f, mapPosition.y - 8.0f,
                             16.0f, 16.0f},
                            0.35f, Fade(RED, 0.85f), 1.5f);
    }

    const float playerSize = std::min(map.width, map.height) / 5.0f;
    drawCornerRectangle(
        {playerPosition.x - playerSize / 2.0f,
         playerPosition.y - playerSize / 2.0f,
         playerSize, playerSize},
        0.35f, gridColor);

    EndScissorMode();

    DrawRectangleLinesEx(map, 2, gridColor);
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

void Graphics::drawCornerRectangle(const Rectangle &area, float edgePercentage,
                                   Color color, float lineThickness)
{
    const float edgeLength = std::min(area.width, area.height) *
                             Clamp(edgePercentage, 0.0f, 1.0f);
    const float right = area.x + area.width;
    const float bottom = area.y + area.height;

    DrawLineEx({area.x, area.y}, {area.x + edgeLength, area.y},
               lineThickness, color);
    DrawLineEx({area.x, area.y}, {area.x, area.y + edgeLength},
               lineThickness, color);

    DrawLineEx({right, area.y}, {right - edgeLength, area.y},
               lineThickness, color);
    DrawLineEx({right, area.y}, {right, area.y + edgeLength},
               lineThickness, color);

    DrawLineEx({area.x, bottom}, {area.x + edgeLength, bottom},
               lineThickness, color);
    DrawLineEx({area.x, bottom}, {area.x, bottom - edgeLength},
               lineThickness, color);

    DrawLineEx({right, bottom}, {right - edgeLength, bottom},
               lineThickness, color);
    DrawLineEx({right, bottom}, {right, bottom - edgeLength},
               lineThickness, color);
}

Vector2 Graphics::translateToMap(Vector2 worldPosition, const Rectangle &map) const
{
    return {
        map.x + worldPosition.x / worldDimensions.x * map.width,
        map.y + worldPosition.y / worldDimensions.y * map.height};
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