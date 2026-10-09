#pragma once
#include <string>

// TODO shouldnt be plural
enum class UpgradeTypes
{
    Speed,
    ContactDamage,
    MaxHealth,
    ProjectileDamage,
    ProjectileCooldown,
    Num,
};

struct UpgradeData
{
    UpgradeTypes upgradeType;
    std::string name;
    std::string description;
};