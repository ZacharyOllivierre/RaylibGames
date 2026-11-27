#pragma once

#include "../game/clock.h"
#include "../game/gameState.h"

#include <string>
#include <vector>

struct WindowDefinition
{
    GameState state;
    std::string name;
    GamePhase activePhase;
};

class WindowManager
{
public:
    WindowManager()
    {
        addWindow(GameState::TradingFloor, "TRADING FLOOR", GamePhase::Trading);
        addWindow(GameState::AfterMarket, "AFTER MARKET", GamePhase::AfterHours);
        addWindow(GameState::Casino, "CASINO", GamePhase::Casino);
    }

    void addWindow(GameState state, const std::string &name, GamePhase activePhase)
    {
        windows.push_back({state, name, activePhase});
    }

    void switchToNext()
    {
        if (!windows.empty())
            currentIndex = (currentIndex + 1) % windows.size();
    }

    void reset()
    {
        currentIndex = 0;
    }

    const WindowDefinition &current() const
    {
        return windows[currentIndex];
    }

    bool canUseCurrentWindow(GamePhase phase) const
    {
        return !windows.empty() && current().activePhase == phase;
    }

private:
    std::vector<WindowDefinition> windows;
    std::size_t currentIndex = 0;
};
