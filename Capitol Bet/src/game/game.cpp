#include "game.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <sstream>
#include <algorithm>

namespace
{
    std::uint32_t createRandomSeed()
    {
        std::random_device randomDevice;
        return static_cast<std::uint32_t>(randomDevice());
    }

    std::string formatShares(double shares)
    {
        char text[32];
        std::snprintf(text, sizeof(text), "%.6g", shares);
        return text;
    }

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
      stockTickMinutes(stockTickMinutes),
      random(createRandomSeed()),
      rouletteWheel(random),
      graphics(windowDimensions, &renderManager)
{
    rouletteWheel.setVerticalScale(
        (windowDimensions.x / 960.0f) / (windowDimensions.y / 540.0f));
    syncGraphics();
}

void Game::update(float delta)
{
    readTerminalInput();
    if (appState != AppState::MainMenu &&
        appState != AppState::GameOverBad &&
        player.money() <= 0.0)
    {
        showGameOver(false, "You ran out of cash.");
    }

    const bool spinWasActive = rouletteWheel.isSpinning();
    rouletteWheel.update(delta);
    if (spinWasActive && !rouletteWheel.isSpinning())
    {
        const double payout = rouletteWheel.payout();
        if (payout > 0.0 && !player.credit(payout))
            terminalMessage = "Roulette win could not be credited.";
        else
            terminalMessage = rouletteWheel.status();
        syncGraphics();
    }

    if (appState == AppState::Game)
    {
        updateManager.update(delta);
        updatePlayerStatus();

        const std::uint64_t tradingDayIndex = clock.tradingDayIndex();
        if (tradingDayIndex > lastBribeDayIndex)
        {
            lastBribeDayIndex = tradingDayIndex;
            showBribeScreen();
        }

        graphics.setWindowAvailability(
            windowManager.canUseCurrentWindow(clock.phase()),
            windowManager.current().activePhase);
    }

    graphics.update(delta);
}

void Game::readTerminalInput()
{
    if (appState == AppState::MainMenu)
    {
        if (IsKeyPressed(KEY_ENTER))
            startNewGame();
        return;
    }

    if (appState == AppState::GameOverBad ||
        appState == AppState::GameOverWin)
    {
        if (IsKeyPressed(KEY_ENTER))
            returnToMainMenu();
        return;
    }

    if (appState == AppState::Help)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            appState = AppState::Game;
            terminalInput.clear();
            terminalMessage.clear();
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
    {
        if (appState == AppState::Bribe)
            resolveBribe(terminalInput);
        else
            executeCommand();
    }

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

    terminalMessage.clear();

    if (command == "switch window")
    {
        switchWindow();
        terminalMessage = "Switched window.";
    }
    else if (command == "switch stock")
    {
        switchStock();
        terminalMessage = "Selected " + stocks[selectedStockIndex].symbol() + ".";
    }
    else if (command.rfind("change speed ", 0) == 0)
    {
        terminalMessage = changeSpeed(command)
                              ? "Simulation speed updated."
                              : "Invalid speed. Use change speed 0x to 20x.";
    }
    else if (command == "reset speed")
    {
        command = "change speed 1x";
        changeSpeed(command);
        terminalMessage = "Simulation speed reset.";
    }
    else if (command == "help")
    {
        appState = AppState::Help;
        terminalMessage.clear();
    }
    else if (command.rfind("advance time", 0) == 0)
    {
        advanceTime(command);
    }
    else if (command.rfind("buy ", 0) == 0)
    {
        trade(command, true);
    }
    else if (command.rfind("sell ", 0) == 0)
    {
        trade(command, false);
    }
    else if (command.rfind("bet ", 0) == 0)
    {
        placeRouletteBet(command);
    }
    else if (command.rfind("flag ", 0) == 0)
    {
        const std::string flagKey = command.substr(5);

        if (!windowManager.canUseCurrentWindow(clock.phase()))
            terminalMessage = "This window is view-only right now.";
        else if (windowManager.current().state != GameState::TradingFloor)
            terminalMessage = "Flags can only be used on the trading floor.";
        else if (!flagKey.empty() && selectedStockIndex < stocks.size())
        {
            stocks[selectedStockIndex].toggleFlag(flagKey);
            terminalMessage = "Price flag updated.";
        }
        else
            terminalMessage = "Usage: flag LABEL.";
    }
    else if (!command.empty())
    {
        terminalMessage = "Unknown command. Type help to see all commands.";
    }

    terminalInput.clear();
    syncGraphics();
}

void Game::advanceTime(const std::string &command)
{
    std::istringstream input(command);
    std::string action;
    std::string timeAction;
    std::string target;
    std::string extra;
    input >> action >> timeAction >> target;

    if (action != "advance" || timeAction != "time" ||
        target.size() != 5 || target[2] != ':' || (input >> extra))
    {
        terminalMessage = "Usage: advance time HH:MM (24:00 means midnight).";
        return;
    }

    for (std::size_t index = 0; index < target.size(); ++index)
    {
        if (index == 2)
            continue;
        if (!std::isdigit(static_cast<unsigned char>(target[index])))
        {
            terminalMessage = "Usage: advance time HH:MM (24:00 means midnight).";
            return;
        }
    }

    int hour = (target[0] - '0') * 10 + target[1] - '0';
    const int minute = (target[3] - '0') * 10 + target[4] - '0';
    if (hour == 24 && minute == 0)
        hour = 0;
    else if (hour >= 24 || minute >= 60)
    {
        terminalMessage = "Invalid time. Use HH:MM from 00:00 to 24:00.";
        return;
    }

    if (!clock.advanceToNextTime(hour, minute))
    {
        terminalMessage = "Invalid time. Use HH:MM from 00:00 to 24:00.";
        return;
    }

    terminalMessage = "Advanced to " + clock.formatTime() + ".";
}

void Game::placeRouletteBet(const std::string &command)
{
    if (windowManager.current().state != GameState::Casino)
    {
        terminalMessage = "Roulette bets can only be placed in the casino.";
        return;
    }
    if (!windowManager.canUseCurrentWindow(clock.phase()))
    {
        terminalMessage = "Roulette is only available during casino hours.";
        return;
    }
    if (rouletteWheel.isSpinning())
    {
        terminalMessage = "Wait for the current roulette spin to finish.";
        return;
    }

    std::istringstream input(command);
    std::string action;
    std::string colorText;
    std::string amountText;
    std::string extra;
    input >> action >> colorText >> amountText;
    if (colorText.empty() || amountText.empty() || (input >> extra))
    {
        terminalMessage = "Usage: bet COLOR AMOUNT (red, black, or green).";
        return;
    }

    RouletteColor color;
    if (colorText == "red")
        color = RouletteColor::Red;
    else if (colorText == "black")
        color = RouletteColor::Black;
    else if (colorText == "green")
        color = RouletteColor::Green;
    else
    {
        terminalMessage = "Choose red, black, or green.";
        return;
    }

    char *end = nullptr;
    const double amount = std::strtod(amountText.c_str(), &end);
    if (end == amountText.c_str() || *end != '\0' ||
        !std::isfinite(amount) || amount <= 0.0)
    {
        terminalMessage = "Enter a valid positive bet amount.";
        return;
    }
    if (amount > player.money())
    {
        terminalMessage = "You do not have enough cash for that bet.";
        return;
    }
    if (!player.spend(amount))
    {
        terminalMessage = "Unable to place that bet.";
        return;
    }
    if (!rouletteWheel.startSpin(color, amount))
    {
        terminalMessage = player.credit(amount)
                              ? "Unable to start the roulette spin."
                              : "Unable to start the spin or refund the bet.";
        return;
    }

    terminalMessage = "Bet placed.";
}

void Game::resolveBribe(const std::string &input)
{
    std::istringstream commandInput(input);
    std::string command;
    std::string extra;
    commandInput >> command;
    std::transform(command.begin(), command.end(), command.begin(),
                   [](unsigned char character)
                   {
                       return static_cast<char>(std::tolower(character));
                   });

    terminalInput.clear();
    if (commandInput >> extra)
    {
        terminalMessage = "Type PAY or REJECT.";
        syncGraphics();
        return;
    }

    if (command == "reject" || command == "decline")
    {
        showGameOver(false, "You rejected the payment demand.");
        return;
    }
    if (command != "pay" && command != "accept")
    {
        terminalMessage = "Type PAY or REJECT.";
        syncGraphics();
        return;
    }
    if (!player.spend(bribeAmount))
    {
        showGameOver(false, "You could not afford the required payment.");
        return;
    }

    ++successfulBribes;
    if (successfulBribes == totalBribeScreens)
    {
        showGameOver(true, "You paid all five demands and survived.");
        return;
    }

    bribeAmount *= 8.0;
    appState = AppState::Game;
    terminalMessage = "Payment accepted. Keep playing until the next trading day.";
    syncGraphics();
}

void Game::trade(const std::string &command, bool isBuy)
{
    if (windowManager.current().state != GameState::TradingFloor)
    {
        terminalMessage = "Stocks can only be traded on the trading floor.";
        return;
    }
    if (!windowManager.canUseCurrentWindow(clock.phase()))
    {
        terminalMessage = "Trading is only available during trading hours.";
        return;
    }

    std::istringstream input(command);
    std::string action;
    std::string symbol;
    double shares = 1.0;
    std::string sharesText;
    std::string extra;
    input >> action >> symbol;
    if (symbol.empty())
    {
        terminalMessage = "Usage: " + action + " SYMBOL [shares].";
        return;
    }

    if (input >> sharesText)
    {
        char *end = nullptr;
        shares = std::strtod(sharesText.c_str(), &end);
        if (end == sharesText.c_str() || *end != '\0' ||
            !std::isfinite(shares) || (input >> extra) || shares <= 0.0)
        {
            terminalMessage = "Usage: " + action + " SYMBOL [shares].";
            return;
        }
    }

    const auto stock = std::find_if(
        stocks.begin(), stocks.end(),
        [&symbol](const Stock &item)
        {
            std::string stockSymbol = item.symbol();
            std::transform(
                stockSymbol.begin(), stockSymbol.end(), stockSymbol.begin(),
                [](unsigned char character)
                { return static_cast<char>(std::tolower(character)); });
            return stockSymbol == symbol;
        });
    if (stock == stocks.end())
    {
        terminalMessage = "Unknown stock: " + symbol + ".";
        return;
    }

    const double price = stock->price();
    const bool success = isBuy
                             ? player.buy(stock->symbol(), price, shares)
                             : player.sell(stock->symbol(), price, shares);
    if (!success)
    {
        if (isBuy)
            terminalMessage = "Not enough cash to buy those shares.";
        else if (player.sharesOwned(stock->symbol()) < shares)
            terminalMessage = "You do not own enough " + stock->symbol() + " shares.";
        else
            terminalMessage = "Unable to complete sale.";
        return;
    }

    terminalMessage = std::string(isBuy ? "Bought " : "Sold ") +
                      formatShares(shares) + " " + stock->symbol() + ".";
}

void Game::switchStock()
{
    if (companies.empty())
        return;

    selectedStockIndex =
        (selectedStockIndex + 1) % companies.size();
}

bool Game::changeSpeed(const std::string &command)
{
    const std::string speedText = command.substr(13);
    if (speedText.size() < 2 || speedText.back() != 'x')
        return false;

    const std::string numberText = speedText.substr(0, speedText.size() - 1);
    char *end = nullptr;
    float speed = std::strtof(numberText.c_str(), &end);

    if (end == numberText.c_str() ||
        *end != '\0' ||
        !std::isfinite(speed) ||
        speed < 0.0f)
        return false;

    const float maxSpeed = 20.0;
    const float minSpeed = 1.0;

    speed = std::clamp(speed, minSpeed, maxSpeed);

    clock.setSimulationSpeed(speed);
    return true;
}

void Game::startNewGame()
{
    resetGame();
    showBribeScreen();
}

void Game::resetGame()
{
    updateManager.clear();
    renderManager.clear();
    stocks.clear();
    companies.clear();
    rouletteWheel.reset();

    clock.reset();
    player = Player();
    windowManager.reset();
    selectedStockIndex = 0;
    bribeAmount = initialBribeAmount;
    successfulBribes = 0;
    lastBribeDayIndex = 0;
    terminalInput.clear();
    terminalMessage.clear();
    gameOverMessage.clear();

    initializeCompanies();
    initializeStocks();
    updateManager.add(&clock);
    for (Stock &stock : stocks)
        updateManager.add(&stock);
    renderManager.add(&clock);
}

void Game::showBribeScreen()
{
    appState = AppState::Bribe;
    terminalInput.clear();
    terminalMessage.clear();
    syncGraphics();
}

void Game::showGameOver(bool won, const std::string &message)
{
    rouletteWheel.reset();
    appState = won ? AppState::GameOverWin : AppState::GameOverBad;
    gameOverMessage = message;
    terminalInput.clear();
    terminalMessage.clear();
    syncGraphics();
}

void Game::returnToMainMenu()
{
    appState = AppState::MainMenu;
    terminalInput.clear();
    terminalMessage.clear();
    gameOverMessage.clear();
    syncGraphics();
}

void Game::switchWindow()
{
    windowManager.switchToNext();
}

void Game::syncGraphics()
{
    const bool tradingFloor =
        appState == AppState::Game &&
        windowManager.current().state == GameState::TradingFloor;

    for (Stock &stock : stocks)
        renderManager.remove(&stock);
    renderManager.remove(&rouletteWheel);

    if (tradingFloor && !stocks.empty())
    {
        selectedStockIndex %= stocks.size();
        renderManager.add(&stocks[selectedStockIndex]);
    }
    else if (appState == AppState::Game &&
             windowManager.current().state == GameState::Casino)
        renderManager.add(&rouletteWheel);

    graphics.updateAppState(appState);
    graphics.updateWindowName(windowManager.current().name);
    graphics.setWindowAvailability(
        windowManager.canUseCurrentWindow(clock.phase()),
        windowManager.current().activePhase);
    graphics.setTerminalText(terminalInput);
    graphics.setTerminalMessage(terminalMessage);
    graphics.setBribeDetails(
        successfulBribes + 1, totalBribeScreens, bribeAmount, player.money());
    graphics.setGameOverMessage(gameOverMessage);
    updatePlayerStatus();
}

void Game::updatePlayerStatus()
{
    std::vector<PlayerPositionDisplay> positions;
    positions.reserve(player.holdings().size());

    for (const StockHolding &holding : player.holdings())
    {
        const auto stock = std::find_if(
            stocks.begin(), stocks.end(),
            [&holding](const Stock &item)
            { return item.symbol() == holding.symbol; });
        if (stock == stocks.end())
            continue;

        const double marketValue = holding.shares * stock->price();
        const double gainLossPercent =
            (marketValue - holding.costBasis) / holding.costBasis * 100.0;
        positions.push_back(
            {holding.symbol, holding.shares, gainLossPercent, marketValue});
    }

    graphics.setPlayerStatus(player.money(), positions);
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

void Game::initializeStocks()
{
    for (std::size_t index = 0; index < companies.size(); ++index)
    {
        stocks.emplace_back(
            createStockConfig(
                companies[index], static_cast<std::uint32_t>(index + 1)),
            clock,
            stockTickMinutes);
    }
}