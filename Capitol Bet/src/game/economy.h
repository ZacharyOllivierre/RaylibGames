#pragma once

// TODO big world update

#include <array>
#include <cstddef>
#include <string>

enum class WorldValue
{
    Prosperity,
    Stability,
    Confidence,
    Competition,
    Innovation,
    Regulation,
    PublicTrust,
    ResourceAbundance,
    Count
};

constexpr std::size_t worldValueCount()
{
    return static_cast<std::size_t>(WorldValue::Count);
}

struct WorldState
{
    std::array<float, worldValueCount()> values{};
};

struct CompanyAttributes
{
    float financialHealth = 50.0f;
    float reputation = 50.0f;
    float efficiency = 50.0f;
    float growthPotential = 50.0f;
    float marketPerception = 50.0f;
};

struct Company
{
    std::string name;
    std::string ticker;
    float stockPrice = 100.0f;
    CompanyAttributes attributes;
};

struct Person
{
    std::string name;
    std::size_t companyIndex = 0;
    std::string role;
    float riskTolerance = 50.0f;
    float ambition = 50.0f;
    float ethics = 50.0f;
    float stress = 0.0f;
};
