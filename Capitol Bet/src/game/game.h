#pragma once

#include "appState.h"
#include "gameState.h"
#include "clock.h"
#include "stock.h"
#include "economy.h"
#include "player.h"
#include "random.h"
#include "roulette.h"
#include "../graphics/graphics.h"
#include "../managers/renderManager.h"
#include "../managers/updateManager.h"
#include "../managers/windowManager.h"

#include <cstdint>
#include <string>
#include <vector>

class Game
{
public:
    Game(Vector2 windowDimensions, float stockTickMinutes);

    void update(float delta);

private:
    static constexpr int totalBribeScreens = 5;
    static constexpr double initialBribeAmount = 100.0;

    void readTerminalInput();
    void executeCommand();
    void resolveBribe(const std::string &command);
    void switchWindow();
    void switchStock();
    void trade(const std::string &command, bool isBuy);
    void placeRouletteBet(const std::string &command);
    void advanceTime(const std::string &command);
    bool changeSpeed(const std::string &command);
    void startNewGame();
    void resetGame();
    void showBribeScreen();
    void showGameOver(bool won, const std::string &message);
    void returnToMainMenu();
    void syncGraphics();
    void updatePlayerStatus();
    void initializeCompanies();
    void initializeStocks();

    UpdateManager updateManager;
    RenderManager renderManager;
    WindowManager windowManager;

    GameClock clock;
    float stockTickMinutes;
    std::vector<Company> companies;
    std::vector<Stock> stocks;
    std::size_t selectedStockIndex = 0;
    Player player;
    RandomSource random;
    RouletteWheel rouletteWheel;
    Graphics graphics;

    AppState appState = AppState::MainMenu;

    std::string terminalInput;
    std::string terminalMessage;
    std::string gameOverMessage;
    double bribeAmount = 100.0;
    int successfulBribes = 0;
    std::uint64_t lastBribeDayIndex = 0;
};
