#pragma once

#include "appState.h"
#include "gameState.h"
#include "clock.h"
#include "stock.h"
#include "economy.h"
#include "../graphics/graphics.h"
#include "../managers/renderManager.h"
#include "../managers/updateManager.h"

#include <string>
#include <vector>

class Game
{
public:
    Game(Vector2 windowDimensions, float stockTickMinutes);

    void update(float delta);

private:
    void readTerminalInput();
    void executeCommand();
    void switchWindow();
    void switchStock();
    void changeSpeed(const std::string &command);
    void syncGraphics();
    void initializeCompanies();

    UpdateManager updateManager;
    RenderManager renderManager;

    GameClock clock;
    std::vector<Company> companies;
    std::vector<Stock> stocks;
    std::size_t selectedStockIndex = 0;
    Graphics graphics;

    AppState appState = AppState::MainMenu;

    GameState gameState = GameState::TradingFloor;

    std::string terminalInput;
};
