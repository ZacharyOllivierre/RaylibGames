#pragma once

#include "raylib.h"
#include "../interface/updateable.h"

#include "../managers/renderManager.h"
#include "../game/appState.h"
#include "../game/gameState.h"

#include <string>

class Graphics : public Updateable
{
public:
    Graphics(Vector2 windowDimensions, RenderManager *rManager);
    ~Graphics() override;

    void update(float delta) override;
    void updateAppState(AppState &state);
    void updateGameState(GameState state);
    void setTerminalText(const std::string &text);

private:
    void draw();
    void drawMainMenu();
    void drawGame();
    void drawHud();
    void drawTerminal();

    RenderManager *rManager;
    Vector2 windowDimensions;
    Vector2 renderDimensions;
    RenderTexture2D hudTexture;
    RenderTexture2D screenTexture;
    AppState state;
    GameState gameState;
    std::string terminalText;
};
