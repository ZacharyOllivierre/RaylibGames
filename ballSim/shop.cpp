#include "shop.h"

Shop::Shop(Player *p)
{
    player = p;

    int itemCount = static_cast<int>(ShopItem::COUNT);
    for (int i = 0; i < itemCount; i++)
    {
        purchaseCounts.push_back(0);
        prices.push_back(getStarterPrice(static_cast<ShopItem>(i)));
    }
}

bool Shop::purchaseItem(ShopItem item)
{
    int index = static_cast<int>(item);
    int price = prices[index];

    if (price > player->getPoints())
    {
        return false;
    }

    purchaseCounts[index]++;
    updatePrices();

    player->subtractPoints(price);

    return true;
}

int Shop::getStarterPrice(ShopItem item)
{
    switch (item)
    {
    case ShopItem::AddBall:
        return 20;
    case ShopItem::IncreaseBounce:
        return 10;
    case ShopItem::ReduceFriction:
        return 10;
    case ShopItem::ReduceGravity:
        return 20;
    case ShopItem::JoltPercent:
        return 15;
    case ShopItem::COUNT:
        break;
    }

    return -1;
}

void Shop::updatePrices()
{
    int size = prices.size();
    for (int i = 0; i < size; i++)
    {
        int starterPrice = getStarterPrice(static_cast<ShopItem>(i));

        prices[i] = starterPrice * (purchaseCounts[i] * purchaseCounts[i]);
    }
}