#include "player.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>

namespace
{
    std::string normalizedSymbol(const std::string &symbol)
    {
        std::string normalized = symbol;
        std::transform(
            normalized.begin(), normalized.end(), normalized.begin(),
            [](unsigned char character)
            {
                return static_cast<char>(std::toupper(character));
            });
        return normalized;
    }
}

Player::Player(double startingMoney)
    : currentMoney(std::isfinite(startingMoney)
                       ? std::max(0.0, startingMoney)
                       : 0.0)
{
}

bool Player::buy(const std::string &symbol, double price, double shares)
{
    if (symbol.empty() || !std::isfinite(price) || price <= 0.0 || shares <= 0)
        return false;

    const double cost = price * shares;
    if (!std::isfinite(cost) || cost > currentMoney)
        return false;

    const std::string normalized = normalizedSymbol(symbol);
    const auto holding = std::find_if(
        currentHoldings.begin(), currentHoldings.end(),
        [&normalized](const StockHolding &item)
        { return normalizedSymbol(item.symbol) == normalized; });

    if (holding == currentHoldings.end())
        currentHoldings.push_back({symbol, shares, cost});
    else
    {
        const double totalShares = holding->shares + shares;
        const double totalCostBasis = holding->costBasis + cost;
        if (!std::isfinite(totalShares) || !std::isfinite(totalCostBasis))
            return false;

        holding->shares = totalShares;
        holding->costBasis = totalCostBasis;
    }

    currentMoney -= cost;
    return true;
}

bool Player::sell(const std::string &symbol, double price, double shares)
{
    if (symbol.empty() || !std::isfinite(price) || price <= 0.0 || shares <= 0)
        return false;

    const std::string normalized = normalizedSymbol(symbol);
    const auto holding = std::find_if(
        currentHoldings.begin(), currentHoldings.end(),
        [&normalized](const StockHolding &item)
        { return normalizedSymbol(item.symbol) == normalized; });

    if (holding == currentHoldings.end())
        return false;

    const double tolerance =
        std::numeric_limits<double>::epsilon() *
        std::max(shares, holding->shares) * 8.0;
    if (shares > holding->shares + tolerance)
        return false;

    const bool sellsAll = shares >= holding->shares - tolerance;
    const double sharesSold = sellsAll ? holding->shares : shares;
    const double proceeds = price * sharesSold;
    const double updatedMoney = currentMoney + proceeds;
    if (!std::isfinite(proceeds) || !std::isfinite(updatedMoney))
        return false;

    currentMoney = updatedMoney;
    if (sellsAll)
        currentHoldings.erase(holding);
    else
    {
        const double remainingFraction =
            (holding->shares - sharesSold) / holding->shares;
        holding->shares -= sharesSold;
        holding->costBasis *= remainingFraction;
    }

    return true;
}

bool Player::spend(double amount)
{
    if (!std::isfinite(amount) || amount <= 0.0 || amount > currentMoney)
        return false;

    currentMoney -= amount;
    return true;
}

bool Player::credit(double amount)
{
    if (!std::isfinite(amount) || amount <= 0.0 ||
        !std::isfinite(currentMoney + amount))
        return false;

    currentMoney += amount;
    return true;
}

double Player::money() const
{
    return currentMoney;
}

const std::vector<StockHolding> &Player::holdings() const
{
    return currentHoldings;
}

double Player::sharesOwned(const std::string &symbol) const
{
    const std::string normalized = normalizedSymbol(symbol);
    const auto holding = std::find_if(
        currentHoldings.begin(), currentHoldings.end(),
        [&normalized](const StockHolding &item)
        { return normalizedSymbol(item.symbol) == normalized; });

    return holding == currentHoldings.end() ? 0.0 : holding->shares;
}
