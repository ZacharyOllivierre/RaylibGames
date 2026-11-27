#include "game.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>

namespace
{
    StockConfig createStockConfig(const Company &company,
                                  std::uint32_t randomSeed)
    {
        StockConfig config;
        config.name = company.name;
        config.symbol = company.ticker;
        config.startingPrice = company.stockPrice;
        config.startingDeviation = 0.028f;
        config.spikeChance = 0.35f;
        config.spikeDeviation = 0.095f;
        config.randomSeed = randomSeed;
        config.chart = {0, 0, 960, 270};
        return config;
    }
}

Game::Game(Vector2 windowDimensions, float stockTickMinutes)
    : clock(6.0f, 60.0f),
      graphics(windowDimensions, &renderManager)
{
    initializeCompanies();
    for (std::size_t index = 0; index < companies.size(); ++index)
        stocks.emplace_back(createStockConfig(
                                companies[index], static_cast<std::uint32_t>(index + 1)),
                            clock,
                            stockTickMinutes);

    updateManager.add(&clock);
    for (Stock &stock : stocks)
    {
        updateManager.add(&stock);
    }
    updateManager.add(&graphics);
    renderManager.add(&clock);
    syncGraphics();
}

void Game::update(float delta)
{
    readTerminalInput();
    updateManager.update(delta);
}

void Game::readTerminalInput()
{
    if (appState == AppState::MainMenu)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            switchWindow();
            syncGraphics();
        }
        return;
    }

    int character = GetCharPressed();
    while (character > 0)
    {
        if (character >= 32 && character <= 126)
            terminalInput += static_cast<char>(character);
        character = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !terminalInput.empty())
        terminalInput.pop_back();
    if (IsKeyPressed(KEY_ENTER))
        executeCommand();

    graphics.setTerminalText(terminalInput);
}

void Game::executeCommand()
{
    std::string command = terminalInput;
    std::transform(command.begin(), command.end(), command.begin(),
                   [](unsigned char character)
                   {
                       return static_cast<char>(std::tolower(character));
                   });

    if (command == "switch window")
        switchWindow();
    else if (command == "switch stock")
        switchStock();
    else if (command.rfind("change speed ", 0) == 0)
        changeSpeed(command);
    else if (command == "reset speed")
    {
        command = "change speed 1x";
        changeSpeed(command);
    }
    else if (command.rfind("flag ", 0) == 0)
    {
        const std::string flagKey = command.substr(5);

        if (!flagKey.empty() &&
            selectedStockIndex < stocks.size())
        {
            stocks[selectedStockIndex].toggleFlag(flagKey);
        }
    }

    terminalInput.clear();
    syncGraphics();
}

void Game::switchStock()
{
    if (companies.empty())
        return;

    selectedStockIndex =
        (selectedStockIndex + 1) % companies.size();
}

void Game::changeSpeed(const std::string &command)
{
    const std::string speedText = command.substr(13);
    if (speedText.size() < 2 || speedText.back() != 'x')
        return;

    const std::string numberText = speedText.substr(0, speedText.size() - 1);
    char *end = nullptr;
    const float speed = std::strtof(numberText.c_str(), &end);

    if (end == numberText.c_str() ||
        *end != '\0' ||
        !std::isfinite(speed) ||
        speed < 0.0f)
        return;

    clock.setSimulationSpeed(speed);
}

void Game::switchWindow()
{
    if (appState == AppState::MainMenu)
    {
        appState = AppState::Game;
        gameState = GameState::TradingFloor;
        return;
    }

    appState = AppState::Game;
    if (gameState == GameState::TradingFloor)
        gameState = GameState::AfterMarket;
    else if (gameState == GameState::AfterMarket)
        gameState = GameState::Casino;
    else
        gameState = GameState::TradingFloor;
}

void Game::syncGraphics()
{
    const bool tradingFloor =
        appState == AppState::Game &&
        gameState == GameState::TradingFloor;

    for (Stock &stock : stocks)
        renderManager.remove(&stock);

    if (tradingFloor && !stocks.empty())
    {
        selectedStockIndex %= stocks.size();
        renderManager.add(&stocks[selectedStockIndex]);
    }

    graphics.updateAppState(appState);
    graphics.updateGameState(gameState);
    graphics.setTerminalText(terminalInput);
}

void Game::initializeCompanies()
{
    Company northstar;
    northstar.name = "Northstar Energy";
    northstar.ticker = "NSE";
    northstar.stockPrice = 82.0f;
    companies.push_back(northstar);

    Company atlas;
    atlas.name = "Atlas Logistics";
    atlas.ticker = "ATL";
    atlas.stockPrice = 116.0f;
    companies.push_back(atlas);

    Company pioneer;
    pioneer.name = "Pioneer Foods";
    pioneer.ticker = "PFD";
    pioneer.stockPrice = 64.0f;
    companies.push_back(pioneer);
}