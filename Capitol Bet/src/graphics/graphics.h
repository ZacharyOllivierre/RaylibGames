#pragma once

#include "raylib.h"
#include "../interface/updateable.h"

#include "../managers/renderManager.h"
#include "../game/appState.h"
#include "../game/clock.h"
#include "../game/player.h"

#include <string>
#include <vector>

struct PlayerPositionDisplay
{
    std::string symbol;
    double shares;
    double gainLossPercent;
    double marketValue;
};

class Graphics : public Updateable
{
public:
    Graphics(Vector2 windowDimensions, RenderManager *rManager);
    ~Graphics() override;

    void update(float delta) override;
    void updateAppState(AppState &state);
    void updateWindowName(const std::string &name);
    void setTerminalText(const std::string &text);
    void setTerminalMessage(const std::string &text);
    void setWindowAvailability(bool available, GamePhase activePhase);
    void setPlayerStatus(
        double money, const std::vector<PlayerPositionDisplay> &positions);
    void setBribeDetails(int day, int totalDays, double amount, double cash);
    void setGameOverMessage(const std::string &message);

private:
    void draw();
    void drawMainMenu();
    void drawGame();
    void drawHelpScreen();
    void drawBribeScreen();
    void drawGameOverScreen();
    void drawHud();
    void drawTerminal();

    RenderManager *rManager;
    Vector2 windowDimensions;
    Vector2 renderDimensions;
    RenderTexture2D hudTexture;
    RenderTexture2D screenTexture;
    AppState state;
    std::string windowName;
    std::string terminalText;
    std::string terminalMessage;
    std::string windowAvailabilityText;
    std::string playerMoneyText;
    std::string bribeAmountText;
    std::string bribeCashText;
    std::string gameOverMessage;
    int bribeDay = 1;
    int totalBribeDays = 5;
    std::vector<PlayerPositionDisplay> playerPositions;
};
