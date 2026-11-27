#include "graphics.h"

Graphics::Graphics(Vector2 windowDimensions, RenderManager *rManager)
    : rManager(rManager),
      windowDimensions(windowDimensions),
      renderDimensions({960.0f, 540.0f}),
      state(AppState::MainMenu),
      gameState(GameState::TradingFloor)
{
    screenTexture = LoadRenderTexture(
        static_cast<int>(renderDimensions.x),
        static_cast<int>(renderDimensions.y));
    hudTexture = LoadRenderTexture(
        static_cast<int>(renderDimensions.x),
        static_cast<int>(renderDimensions.y));
}

Graphics::~Graphics()
{
    UnloadRenderTexture(screenTexture);
    UnloadRenderTexture(hudTexture);
}

void Graphics::update(float)
{
    BeginTextureMode(screenTexture);
    ClearBackground(BLACK);

    switch (state)
    {
    case AppState::MainMenu:
        drawMainMenu();
        break;
    case AppState::Game:
        drawGame();
        break;
    }

    EndTextureMode();
    draw();
}

void Graphics::draw()
{
    // TODO fix order too lazy rn sorry future me
    BeginTextureMode(hudTexture);
    ClearBackground(BLANK);
    if (state == AppState::Game)
    {
        drawHud();
        drawTerminal();
    }
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

    const Rectangle destination = {0.0f, 0.0f, windowDimensions.x, windowDimensions.y};
    const Rectangle source = {
        0.0f,
        0.0f,
        static_cast<float>(screenTexture.texture.width),
        -static_cast<float>(screenTexture.texture.height)};

    DrawTexturePro(screenTexture.texture, source, destination, {0.0f, 0.0f}, 0.0f, WHITE);
    DrawTexturePro(hudTexture.texture, source, destination, {0.0f, 0.0f}, 0.0f, WHITE);

    DrawFPS(10, 10);
    EndDrawing();
}

void Graphics::updateAppState(AppState &state)
{
    this->state = state;
}

void Graphics::updateGameState(GameState state)
{
    gameState = state;
}

void Graphics::setTerminalText(const std::string &text)
{
    terminalText = text;
}

void Graphics::drawMainMenu()
{
    DrawText("Capitol Bet", 50, 50, 36, RED);
    DrawText("Main Menu", 50, 105, 22, RAYWHITE);
    DrawRectangleLines(50, 155, 860, 280, RED);
    DrawText("Press ENTER to begin", 75, 235, 20, RAYWHITE);
}

void Graphics::drawGame()
{
    if (rManager != nullptr)
        rManager->render();
    const char *windowName = "TRADING FLOOR";
    if (gameState == GameState::AfterMarket)
        windowName = "AFTER MARKET";
    else if (gameState == GameState::Casino)
        windowName = "CASINO";

    DrawText(windowName, 50, 300, 28, PURPLE);
}

void Graphics::drawTerminal()
{
    DrawRectangle(16, 462, 928, 62, Fade(BLACK, 0.85f));
    DrawRectangleLines(16, 462, 928, 62, PURPLE);
    DrawText("> ", 28, 482, 20, PURPLE);
    DrawText(terminalText.c_str(), 50, 482, 20, RAYWHITE);
}

void Graphics::drawHud()
{
    constexpr int width = 220;
    constexpr int height = 48;
    constexpr int rightMargin = 16;
    constexpr int bottomMargin = 16;
    const int x = static_cast<int>(renderDimensions.x) - width - rightMargin;
    const int y = static_cast<int>(renderDimensions.y) - height - bottomMargin;

    DrawRectangle(x, y, width, height, Fade(BLACK, 0.75f));
    DrawRectangleLines(x, y, width, height, PURPLE);
    DrawText("HUD  |  SPACE / ENTER", x + 13, y + 16, 16, RAYWHITE);
}
