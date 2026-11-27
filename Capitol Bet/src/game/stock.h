#pragma once

#include "raylib.h"
#include "clock.h"
#include "random.h"
#include "../interface/renderable.h"
#include "../interface/updateable.h"

#include <cstdint>
#include <string>
#include <vector>

struct StockConfig
{
    std::string name = "Unnamed Stock";
    std::string symbol = "STK";
    float startingPrice = 100.0f;
    float startingDeviation = 0.008f;
    float spikeChance = 0.04f;
    float spikeDeviation = 0.035f;
    float minimumPrice = 0.01f;
    std::uint32_t randomSeed = 1;
    float minimumZoom = 0.1f;
    float maximumZoom = 1.5f;
    float zoomStep = 0.1f;
    Rectangle chart = {30.0f, 30.0f, 940.0f, 520.0f};
};

struct PriceFlag
{
    std::string key;
    float price;
};

class Stock : public Renderable, public Updateable
{
public:
    Stock(const StockConfig &config,
          const GameClock &clock,
          float stockTickMinutes);

    void update(float delta) override;
    void render() override;

    void toggleFlag(const std::string &key);

    const std::string &symbol() const;
    float price() const;
    float changePercent() const;

private:
    void tick();

    void updateZoom(float delta);

    void drawPanel() const;
    void drawHistory(const Rectangle &plot, Color trendColor) const;
    void drawCurrentPriceGuide(const Rectangle &plot) const;
    void drawHeader(Color trendColor) const;
    void drawFooter() const;

    void drawDashedPriceLine(const Rectangle &plot, float price, const std::string &label, Color color) const;

    float tickInterval() const;
    float worldWidth() const;
    float worldHeight() const;

    float currentGraphX() const;
    float currentGraphPrice() const;
    float graphX(std::size_t index) const;

    float easedProgress() const;

    Vector2 graphPoint(std::size_t index, const Rectangle &plot) const;
    Vector2 graphPoint(float x, float price, const Rectangle &plot) const;
    Vector2 worldToScreen(Vector2 point, const Rectangle &plot) const;

    static std::string formatMoney(float value);
    static std::string formatPercent(float value);

    StockConfig config;
    const GameClock &clock;
    float stockTickMinutes;

    float currentPrice;
    float openingPrice;

    float deviation;
    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float zoom = 1.0f;
    float zoomTarget = 1.0f;

    float lastChange = 0.0f;
    float timeSinceTick = 0.0f;
    float segmentProgress = 0.0f;
    float lastClockHour = 0.0f;
    int tickCount = 0;

    std::vector<float> history;
    std::vector<PriceFlag> flags;
    RandomSource random;
};
