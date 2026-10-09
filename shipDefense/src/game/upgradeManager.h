#pragma once

#include "raylib.h"
#include <algorithm>
#include <array>
#include <vector>
#include <random>

#include "upgradeTypes.h"

// TODO split into .h .cpp

class UpgradeManager
{
public:
    UpgradeManager(int choiceCount)
        : choiceCount(choiceCount)
    {
    }

    // TODO gross
    void generateChoices()
    {
        upgradeChoices.clear();

        std::array<UpgradeTypes, static_cast<std::size_t>(UpgradeTypes::Num)> allUpgrades = {
            UpgradeTypes::Speed,
            UpgradeTypes::ContactDamage,
            UpgradeTypes::MaxHealth,
            UpgradeTypes::ProjectileDamage,
            UpgradeTypes::ProjectileCooldown};

        std::shuffle(allUpgrades.begin(), allUpgrades.end(),
                     std::mt19937{static_cast<unsigned int>(GetRandomValue(0, 1000000))});

        upgradeChoices.assign(
            allUpgrades.begin(),
            allUpgrades.begin() + std::min<int>(choiceCount, allUpgrades.size()));
    }

    const std::vector<UpgradeTypes> &getUpgradeTypes() const
    {
        return upgradeChoices;
    }

    std::vector<UpgradeData> getUpgradeData() const
    {
        std::vector<UpgradeData> list;

        for (UpgradeTypes type : upgradeChoices)
        {
            UpgradeData data{type, "", ""};

            switch (type)
            {
            case UpgradeTypes::Speed:
                data.name = "Speed";
                data.description = "Increases Ship Speed";
                break;

            case UpgradeTypes::ContactDamage:
                data.name = "Spikes";
                data.description = "Increases Collision Damage";
                break;

            case UpgradeTypes::MaxHealth:
                data.name = "Health";
                data.description = "Increases Maximum Health";
                break;

            case UpgradeTypes::ProjectileDamage:
                data.name = "Damage";
                data.description = "Increases Projectile Damage";
                break;

            case UpgradeTypes::ProjectileCooldown:
                data.name = "Cooldown";
                data.description = "Guns Go More BURRR";
                break;

            case UpgradeTypes::Num:
                break;
            }
            list.push_back(data);
        }
        return list;
    }

private:
    int choiceCount;

    std::vector<UpgradeTypes> upgradeChoices;
};