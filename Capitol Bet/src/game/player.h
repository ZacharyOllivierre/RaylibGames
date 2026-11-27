#pragma once

#include <string>
#include <vector>

struct StockHolding
{
    std::string symbol;
    double shares;
    double costBasis;
};

class Player
{
public:
    explicit Player(double startingMoney = 200.0);

    bool buy(const std::string &symbol, double price, double shares);
    bool sell(const std::string &symbol, double price, double shares);
    bool spend(double amount);
    bool credit(double amount);

    double money() const;
    const std::vector<StockHolding> &holdings() const;
    double sharesOwned(const std::string &symbol) const;

private:
    double currentMoney;
    std::vector<StockHolding> currentHoldings;
};
