#include "stock.h"

#include "colors.h"
#include "../graphics/util.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace
{
    constexpr float minimumDeviation = 0.0001f;
    constexpr float minimumTickInterval = 0.001f;
    constexpr float minimumStockTickMinutes = minimumTickInterval * 60.0f;
    constexpr float graphUnitsPerTick = 0.25f;
    constexpr float baseWorldWidth = 16.0f;
    constexpr float baseWorldHeight = 32.0f;
    constexpr float zoomSmoothing = 12.0f;
    constexpr float chartPadding = 16.0f;
    constexpr float lineThickness = 3.0f;
    constexpr float glowThickness = 7.0f;
    constexpr float currentPriceRadius = 5.0f;
    constexpr float currentPriceGlowRadius = 8.0f;
    constexpr int titleSize = 24;
    constexpr int symbolSize = 18;
    constexpr int priceSize = 28;
    constexpr int detailSize = 18;

}

Stock::Stock(const StockConfig &config, const GameClock &clock, float stockTickMinutes)
    : config(config),
      clock(clock),
      stockTickMinutes(std::max(stockTickMinutes, minimumStockTickMinutes)),
      currentPrice(std::max(config.startingPrice, config.minimumPrice)),
      openingPrice(currentPrice),
      random(config.randomSeed)
{
    deviation = std::max(std::abs(config.startingDeviation), minimumDeviation);

    history.reserve(256);
    history.push_back(currentPrice);

    cameraY = currentPrice;
}

void Stock::update(float delta)
{
    if (delta <= 0.0f)
        return;

    const float clockHour = clock.elapsedGameHours();
    const float elapsedGameHours = std::max(0.0f, clockHour - lastClockHour);
    lastClockHour = clockHour;
    timeSinceTick += elapsedGameHours;
    while (timeSinceTick >= tickInterval())
    {
        timeSinceTick -= tickInterval();
        tick();
    }

    segmentProgress = timeSinceTick / tickInterval();
    updateZoom(delta);
    cameraX = currentGraphX();
    cameraY = currentGraphPrice();
}

void Stock::tick()
{
    const float previousPrice = currentPrice;
    const float varianceShock = random.value() * deviation;
    const bool hasSpike = random.chance(config.spikeChance);
    const float spike = hasSpike
                            ? random.value() * config.spikeDeviation
                            : 0.0f;
    const float logReturn = varianceShock + spike;

    currentPrice = std::max(
        config.minimumPrice, currentPrice * std::exp(logReturn));
    lastChange = currentPrice - previousPrice;
    ++tickCount;

    history.push_back(currentPrice);
}

void Stock::updateZoom(float delta)
{
    const float wheel = GetMouseWheelMove();
    if (wheel != 0.0f)
    {
        zoomTarget = std::clamp(
            zoomTarget + wheel * config.zoomStep,
            config.minimumZoom,
            config.maximumZoom);
    }

    const float smoothing = std::min(1.0f, delta * zoomSmoothing);
    zoom += (zoomTarget - zoom) * smoothing;
    zoom = std::clamp(zoom, config.minimumZoom, config.maximumZoom);
}

void Stock::render()
{
    const Rectangle plot = config.chart;
    const Color trendColor =
        currentPrice >= openingPrice ? StockColors::rising : StockColors::falling;

    drawPanel();

    BeginScissorMode(
        static_cast<int>(plot.x),
        static_cast<int>(plot.y),
        static_cast<int>(plot.width),
        static_cast<int>(plot.height));

    DrawRectangleRec(plot, StockColors::plot);
    drawHistory(plot, trendColor);
    drawCurrentPriceGuide(plot);

    for (const PriceFlag &flag : flags)
    {
        drawDashedPriceLine(plot, flag.price, flag.key, StockColors::flag);
    }

    EndScissorMode();
    drawHeader(trendColor);
    drawFooter();
}

void Stock::drawPanel() const
{
    DrawRectangleRec(config.chart, StockColors::panel);
    DrawRectangleLinesEx(config.chart, 2.0f, StockColors::axis);
}

void Stock::drawHistory(const Rectangle &plot, Color trendColor) const
{
    if (history.size() < 2)
        return;

    const std::size_t maxPoints =
        std::max<std::size_t>(2, static_cast<std::size_t>(plot.width * 2.0f));

    const std::size_t stride =
        std::max<std::size_t>(1, (history.size() - 1) / maxPoints);

    std::size_t previousIndex = 0;
    for (std::size_t index = stride; index + 1 < history.size(); index += stride)
    {
        const Vector2 first = graphPoint(previousIndex, plot);
        const Vector2 second = graphPoint(index, plot);

        DrawLineEx(first, second, glowThickness, StockColors::glow);
        DrawLineEx(first, second, lineThickness, trendColor);

        previousIndex = index;
    }

    const std::size_t lastIndex = history.size() - 1;
    const Vector2 previousPoint = graphPoint(previousIndex, plot);
    const float progress = easedProgress();
    const Vector2 currentPoint =
        graphPoint(graphX(lastIndex - 1) + graphUnitsPerTick * progress,
                   history[lastIndex - 1] + (history.back() - history[lastIndex - 1]) * progress, plot);

    DrawLineEx(previousPoint, currentPoint, glowThickness, StockColors::glow);
    DrawLineEx(previousPoint, currentPoint, lineThickness, trendColor);
    DrawCircleV(currentPoint, currentPriceGlowRadius, StockColors::glow);
    DrawCircleV(currentPoint, currentPriceRadius, trendColor);
}

void Stock::drawCurrentPriceGuide(const Rectangle &plot) const
{
    drawDashedPriceLine(
        plot,
        currentGraphPrice(),
        formatMoney(currentGraphPrice()),
        StockColors::mutedText);
}

void Stock::drawDashedPriceLine(const Rectangle &plot, float price,
                                const std::string &label, Color color) const
{
    const Vector2 point = worldToScreen({cameraX, price}, plot);

    for (int x = static_cast<int>(plot.x); x < plot.x + plot.width; x += 14)
    {
        DrawLine(x, static_cast<int>(point.y), std::min(x + 8, static_cast<int>(plot.x + plot.width)),
                 static_cast<int>(point.y), color);
    }

    GraphicsUtil::drawGlowText(
        label,
        {plot.x + plot.width - MeasureText(label.c_str(), detailSize) - 8.0f,
         point.y - 12.0f},
        detailSize,
        color,
        StockColors::glow);
}

void Stock::drawHeader(Color trendColor) const
{
    const float left = config.chart.x + chartPadding;
    const float top = config.chart.y + chartPadding / 2.0f;
    const float right = config.chart.x + config.chart.width - chartPadding;
    const std::string priceText = formatMoney(currentPrice);
    const std::string changeText = formatPercent(changePercent());

    GraphicsUtil::drawGlowText(config.name, {left, top}, titleSize,
                               StockColors::text, StockColors::glow);
    GraphicsUtil::drawGlowText(config.symbol, {left, top + titleSize}, symbolSize,
                               StockColors::mutedText, StockColors::glow);
    GraphicsUtil::drawGlowText(
        priceText, {right - MeasureText(priceText.c_str(), priceSize), top},
        priceSize, trendColor, StockColors::glow);
    GraphicsUtil::drawGlowText(
        changeText, {right - MeasureText(changeText.c_str(), detailSize), top + priceSize},
        detailSize, trendColor, StockColors::glow);
}

void Stock::drawFooter() const
{
    const std::string details =
        "OPEN " + formatMoney(openingPrice) +
        "   CHANGE " + formatMoney(lastChange) +
        "   TICK " + std::to_string(tickCount);
    GraphicsUtil::drawGlowText(
        details,
        {config.chart.x + chartPadding,
         config.chart.y + config.chart.height - chartPadding - detailSize},
        detailSize, StockColors::text, StockColors::glow);
}

float Stock::tickInterval() const
{
    return std::max(stockTickMinutes / 60.0f, minimumTickInterval);
}

float Stock::worldWidth() const
{
    return baseWorldWidth / zoom;
}

float Stock::worldHeight() const
{
    return baseWorldHeight / zoom;
}

float Stock::currentGraphX() const
{
    return history.empty() ? 0.0f
                           : graphX(history.size() - 1) +
                                 graphUnitsPerTick * easedProgress();
}

float Stock::currentGraphPrice() const
{
    if (history.empty())
        return currentPrice;

    const std::size_t lastIndex = history.size() - 1;
    const float progress = easedProgress();
    const float previousPrice = lastIndex == 0 ? history.front() : history[lastIndex - 1];
    return previousPrice + (history.back() - previousPrice) * progress;
}

float Stock::graphX(std::size_t index) const
{
    return static_cast<float>(
               tickCount - static_cast<int>(history.size()) + 1 + index) *
           graphUnitsPerTick;
}

float Stock::easedProgress() const
{
    const float progress = std::clamp(segmentProgress, 0.0f, 1.0f);
    return progress * progress * (3.0f - 2.0f * progress);
}

Vector2 Stock::graphPoint(std::size_t index, const Rectangle &plot) const
{
    return graphPoint(graphX(index), history[index], plot);
}

Vector2 Stock::graphPoint(float x, float price, const Rectangle &plot) const
{
    return worldToScreen({x, price}, plot);
}

Vector2 Stock::worldToScreen(Vector2 point, const Rectangle &plot) const
{
    return {
        plot.x + (point.x - cameraX + worldWidth() / 2.0f) *
                     plot.width / worldWidth(),
        plot.y + (cameraY + worldHeight() / 2.0f - point.y) *
                     plot.height / worldHeight()};
}

void Stock::toggleFlag(const std::string &key)
{
    const auto flag = std::find_if(
        flags.begin(),
        flags.end(),
        [&key](const PriceFlag &item)
        { return item.key == key; });

    if (flag != flags.end())
    {
        flags.erase(flag);
        return;
    }

    flags.push_back({key, currentGraphPrice()});
}

float Stock::changePercent() const
{
    return (currentPrice - openingPrice) / openingPrice * 100.0f;
}

std::string Stock::formatMoney(float value)
{
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "$%.2f", value);
    return buffer;
}

std::string Stock::formatPercent(float value)
{
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%+.2f%%", value);
    return buffer;
}
