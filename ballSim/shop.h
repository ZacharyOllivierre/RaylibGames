#pragma once
#include <vector>
#include <string>
#include "player.h"

using std::vector;

enum class ShopItem
{
    AddBall,
    IncreaseBounce,
    ReduceFriction,
    ReduceGravity,
    JoltPercent,
    COUNT
};

class Shop
{
private:
    Player *player;

    int getStarterPrice(ShopItem item);
    void updatePrices();

public:
    vector<int> purchaseCounts;
    vector<int> prices;

    Shop(Player *p);

    bool purchaseItem(ShopItem item);

    inline int getPrice(ShopItem item) { return prices[static_cast<int>(item)]; }
    inline std::string getPriceS(ShopItem item) { return std::to_string(getPrice(item)); }
};