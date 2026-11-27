#include "graphics.h"

#include <algorithm>
#include <cstdio>

Graphics::Graphics(Vector2 windowDimensions, RenderManager *rManager)
    : rManager(rManager),
      windowDimensions(windowDimensions),
      renderDimensions({960.0f, 540.0f}),
      state(AppState::MainMenu),
      windowName("TRADING FLOOR")
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
    case AppState::Help:
        drawHelpScreen();
        break;
    case AppState::Bribe:
        drawBribeScreen();
        break;
    case AppState::GameOverBad:
    case AppState::GameOverWin:
        drawGameOverScreen();
        break;
    }

    EndTextureMode();
    draw();
}

void Graphics::draw()
{
    BeginTextureMode(hudTexture);
    ClearBackground(BLANK);
    if (state == AppState::Game)
    {
        drawHud();
    }
    if (state == AppState::Game || state == AppState::Bribe)
    {
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

void Graphics::updateWindowName(const std::string &name)
{
    windowName = name;
}

void Graphics::setTerminalText(const std::string &text)
{
    terminalText = text;
}

void Graphics::setTerminalMessage(const std::string &text)
{
    terminalMessage = text;
}

void Graphics::setWindowAvailability(bool available, GamePhase activePhase)
{
    windowAvailabilityText = available
                                 ? "USAGE AVAILABLE"
                                 : std::string("VIEW ONLY - ACTIVE DURING ") +
                                       gamePhaseName(activePhase);
}

void Graphics::setPlayerStatus(
    double money, const std::vector<PlayerPositionDisplay> &positions)
{
    char buffer[48];
    std::snprintf(buffer, sizeof(buffer), "$%.2f", money);
    playerMoneyText = buffer;
    playerPositions = positions;
}

void Graphics::setBribeDetails(int day, int totalDays, double amount, double cash)
{
    char amountBuffer[48];
    char cashBuffer[48];
    std::snprintf(amountBuffer, sizeof(amountBuffer), "$%.2f", amount);
    std::snprintf(cashBuffer, sizeof(cashBuffer), "$%.2f", cash);
    bribeDay = day;
    totalBribeDays = totalDays;
    bribeAmountText = amountBuffer;
    bribeCashText = cashBuffer;
}

void Graphics::setGameOverMessage(const std::string &message)
{
    gameOverMessage = message;
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

    DrawText(windowName.c_str(), 50, 300, 28, PURPLE);
    DrawText(windowAvailabilityText.c_str(), 50, 336, 16, LIGHTGRAY);
}

void Graphics::drawHelpScreen()
{
    DrawText("COMMAND GUIDE", 50, 36, 32, PURPLE);
    DrawRectangle(50, 88, 860, 364, Fade(StockColors::panel, 0.9f));
    DrawRectangleLines(50, 88, 860, 364, PURPLE);

    const char *commands[] = {
        "buy [SYMBOL] [shares]",
        "sell [SYMBOL] [shares]",
        "flag [LABEL]",
        "bet [COLOR] [AMOUNT]",
        "switch window",
        "switch stock",
        "change speed [AMOUNT]x",
        "reset speed",
        "advance time [HH:MM]",
        "help"};

    const char *descriptions[] = {
        "Buy shares on the trading floor.",
        "Sell shares you own.",
        "Toggle flag on the selected stock.",
        "Bet red, black, or green during casino hours.",
        "Move to the next game window.",
        "Move to the next company.",
        "Set simulation speed from 0x to 20x.",
        "Return simulation speed to 1x.",
        "Skip to the next occurrence of that time.",
        "Open this command guide."};

    for (int i = 0; i < 10; i++)
    {
        int y = 106 + i * 27;

        DrawText(commands[i], 76, y, 16, PURPLE);
        DrawText(descriptions[i], 270, y, 16, StockColors::text);
    }

    DrawText("During a bribe: PAY/ACCEPT to continue; REJECT/DECLINE to end the run.",
             76, 386, 16, StockColors::mutedText);

    DrawText("Press ENTER to return.", 76, 420, 16, PURPLE);
}

void Graphics::drawBribeScreen()
{
    DrawText("BRIBE DEMAND", 50, 50, 36, RED);

    char dayText[48];
    std::snprintf(dayText, sizeof(dayText), "TRADING DAY %d OF %d",
                  bribeDay, totalBribeDays);
    DrawText(dayText, 50, 112, 20, LIGHTGRAY);

    DrawRectangle(50, 160, 860, 178, Fade(BLACK, 0.65f));
    DrawRectangleLines(50, 160, 860, 178, PURPLE);
    DrawText("REQUIRED PAYMENT", 76, 184, 18, LIGHTGRAY);
    DrawText(bribeAmountText.c_str(), 76, 216, 32, RAYWHITE);
    DrawText("YOUR CASH", 520, 184, 18, LIGHTGRAY);
    DrawText(bribeCashText.c_str(), 520, 216, 32, RAYWHITE);
    DrawText("Pay to continue. Rejecting or being unable to pay ends the run.",
             76, 285, 18, LIGHTGRAY);
    DrawText("Type PAY or REJECT and press ENTER.", 50, 390, 18, PURPLE);
}

void Graphics::drawGameOverScreen()
{
    const bool won = state == AppState::GameOverWin;
    DrawText(won ? "YOU WIN" : "GAME OVER",
             50, 50, 40, won ? GREEN : RED);
    DrawText(gameOverMessage.c_str(), 50, 132, 22, RAYWHITE);
    DrawText("Press ENTER to return to the main menu.", 50, 212, 18, LIGHTGRAY);
}

void Graphics::drawTerminal()
{
    DrawRectangle(16, 462, 928, 62, Fade(BLACK, 0.85f));
    DrawRectangleLines(16, 462, 928, 62, PURPLE);
    DrawText(terminalMessage.c_str(), 28, 468, 14, LIGHTGRAY);
    DrawText("> ", 28, 493, 20, PURPLE);
    DrawText(terminalText.c_str(), 50, 493, 20, RAYWHITE);
}

void Graphics::drawHud()
{
    constexpr int x = 660;
    constexpr int y = 294;
    constexpr int width = 284;
    constexpr int height = 154;
    constexpr int lineHeight = 19;

    DrawRectangle(x, y, width, height, Fade(BLACK, 0.82f));
    DrawRectangleLines(x, y, width, height, PURPLE);
    DrawText("PLAYER", x + 14, y + 10, 16, PURPLE);
    DrawText("CASH", x + 14, y + 34, 14, LIGHTGRAY);
    DrawText(playerMoneyText.c_str(), x + 78, y + 33, 16, RAYWHITE);
    DrawText("POSITIONS", x + 14, y + 59, 14, LIGHTGRAY);

    const std::size_t visiblePositions =
        std::min<std::size_t>(3, playerPositions.size());
    for (std::size_t index = 0; index < visiblePositions; ++index)
    {
        const PlayerPositionDisplay &position = playerPositions[index];
        char sharesText[32];
        std::snprintf(sharesText, sizeof(sharesText), "%.6g", position.shares);
        char positionText[128];
        std::snprintf(
            positionText, sizeof(positionText), "%s x%s  %+.2f%%  $%.2f",
            position.symbol.c_str(), sharesText,
            position.gainLossPercent, position.marketValue);
        const Color positionColor = position.gainLossPercent > 0.0
                                        ? GREEN
                                    : position.gainLossPercent < 0.0
                                        ? RED
                                        : RAYWHITE;
        DrawText(positionText, x + 14,
                 y + 80 + static_cast<int>(index) * lineHeight,
                 14, positionColor);
    }
    if (playerPositions.empty())
        DrawText("None", x + 14, y + 80, 14, DARKGRAY);
    else if (playerPositions.size() > visiblePositions)
    {
        const std::string moreText =
            "+ " + std::to_string(playerPositions.size() - visiblePositions) + " more";
        DrawText(moreText.c_str(), x + 14, y + 80 + 3 * lineHeight, 14, DARKGRAY);
    }
}
