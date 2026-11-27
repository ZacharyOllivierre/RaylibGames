#include "roulette.h"

#include "rlgl.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace
{
    constexpr Vector2 wheelCenter = {480.0f, 205.0f};
    constexpr float wheelRadius = 160.0f;
    constexpr float degreesPerPocket = 360.0f / 37.0f;

    constexpr Color rouletteRed = {205, 60, 85, 100};
    constexpr Color rouletteBlack = {32, 38, 52, 100};
    constexpr Color rouletteGreen = {50, 145, 125, 100};
    constexpr Color neonPurple = {155, 105, 195, 255};
    constexpr Color neonCyan = {75, 205, 215, 255};

    float normalizedDegrees(float degrees)
    {
        degrees = std::fmod(degrees, 360.0f);
        return degrees < 0.0f ? degrees + 360.0f : degrees;
    }

    Vector2 wheelPoint(float radius, float angle, float verticalScale)
    {
        const float radians = angle * DEG2RAD;
        return {
            wheelCenter.x + std::cos(radians) * radius,
            wheelCenter.y + std::sin(radians) * radius * verticalScale};
    }

    void drawWheelDisk(float radius, float verticalScale, Color color)
    {
        constexpr int segments = 144;
        for (int segment = 0; segment < segments; ++segment)
        {
            const float startAngle =
                static_cast<float>(segment) * 360.0f / segments;
            const float endAngle =
                static_cast<float>(segment + 1) * 360.0f / segments;
            DrawTriangle(
                wheelCenter,
                wheelPoint(radius, startAngle, verticalScale),
                wheelPoint(radius, endAngle, verticalScale),
                color);
        }
    }

    void drawWheelRing(float radius, float verticalScale, float thickness, Color color)
    {
        constexpr int segments = 144;
        for (int segment = 0; segment < segments; ++segment)
        {
            const float startAngle =
                static_cast<float>(segment) * 360.0f / segments;
            const float endAngle =
                static_cast<float>(segment + 1) * 360.0f / segments;
            DrawLineEx(
                wheelPoint(radius, startAngle, verticalScale),
                wheelPoint(radius, endAngle, verticalScale),
                thickness,
                color);
        }
    }
}

RouletteWheel::RouletteWheel(RandomSource &random)
    : random(random)
{
}

void RouletteWheel::update(float delta)
{
    if (!spinning || !std::isfinite(delta) || delta <= 0.0f)
        return;

    spinElapsed = std::min(spinElapsed + delta, spinDuration);
    const float progress = spinElapsed / spinDuration;
    const float easedProgress = 1.0f - std::pow(1.0f - progress, 3.0f);
    const float currentRotation =
        startRotation + (targetRotation - startRotation) * easedProgress;

    if (spinElapsed >= spinDuration)
        finishSpin();
    else
        rotation = normalizedDegrees(currentRotation);
}

void RouletteWheel::render()
{
    drawWheel();
}

bool RouletteWheel::startSpin(RouletteColor color, double amount)
{
    const double multiplier = color == RouletteColor::Green ? 36.0 : 2.0;
    if (spinning || !std::isfinite(amount) || amount <= 0.0 ||
        !std::isfinite(amount * multiplier))
        return false;

    betColor = color;
    wager = amount;
    payoutAmount = 0.0;
    winningSlot = -1;
    spinElapsed = 0.0f;
    startRotation = rotation;

    winningSlot = chooseWinningSlot();
    const float winningPocketCenter =
        static_cast<float>(winningSlot + 0.5f) * degreesPerPocket;
    const float alignment =
        normalizedDegrees(pointerAngle - rotation - winningPocketCenter);
    targetRotation =
        startRotation + static_cast<float>(random.integer(5, 7)) * fullCircle +
        alignment;

    spinning = true;
    char amountText[48];
    std::snprintf(amountText, sizeof(amountText), "Spinning for $%.2f", wager);
    resultText = amountText;
    return true;
}

void RouletteWheel::reset()
{
    wager = 0.0;
    payoutAmount = 0.0;
    spinElapsed = 0.0f;
    winningSlot = -1;
    spinning = false;
    rotation = 0.0f;
    resultText = "Place a bet to spin.";
}

void RouletteWheel::setVerticalScale(float scale)
{
    if (std::isfinite(scale) && scale > 0.0f)
        verticalScale = scale;
}

bool RouletteWheel::isSpinning() const
{
    return spinning;
}

double RouletteWheel::payout() const
{
    return payoutAmount;
}

const std::string &RouletteWheel::status() const
{
    return resultText;
}

void RouletteWheel::drawWheel() const
{
    rlPushMatrix();
    rlTranslatef(0.0f, wheelCenter.y, 0.0f);
    rlScalef(1.0f, verticalScale, 1.0f);
    rlTranslatef(0.0f, -wheelCenter.y, 0.0f);

    for (int slot = 0; slot < 37; ++slot)
    {
        const RouletteColor color = colorForSlot(slot);
        const Color pocketColor = color == RouletteColor::Green
                                      ? rouletteGreen
                                  : color == RouletteColor::Red
                                      ? rouletteRed
                                      : rouletteBlack;
        const float startAngle =
            rotation + static_cast<float>(slot) * degreesPerPocket;
        DrawCircleSector(
            wheelCenter,
            wheelRadius,
            startAngle,
            startAngle + degreesPerPocket,
            8,
            pocketColor);
    }
    rlPopMatrix();

    for (int slot = 0; slot < 37; ++slot)
    {
        const float startAngle =
            rotation + static_cast<float>(slot) * degreesPerPocket;
        const Color spokeColor = slot % 2 == 0 ? neonPurple : neonCyan;
        DrawLineEx(
            wheelPoint(61.0f, startAngle, verticalScale),
            wheelPoint(wheelRadius - 2.0f, startAngle, verticalScale),
            4.0f,
            Fade(spokeColor, 0.14f));
        DrawLineEx(
            wheelPoint(61.0f, startAngle, verticalScale),
            wheelPoint(wheelRadius - 2.0f, startAngle, verticalScale),
            1.0f,
            Fade(spokeColor, 0.65f));
    }

    drawWheelDisk(60.0f, verticalScale, StockColors::panel);
    drawWheelRing(160.0f, verticalScale, 14.0f, Fade(neonPurple, 0.16f));
    drawWheelRing(158.0f, verticalScale, 2.0f, neonPurple);
    drawWheelRing(151.0f, verticalScale, 1.0f, neonCyan);
    drawWheelRing(56.0f, verticalScale, 10.0f, Fade(neonCyan, 0.14f));
    drawWheelRing(54.0f, verticalScale, 2.0f, neonCyan);
    drawWheelRing(47.0f, verticalScale, 1.0f, neonPurple);

    const Vector2 pointerTip =
        wheelPoint(wheelRadius - 1.0f, -90.0f, verticalScale);
    const Vector2 pointerLeft = {
        wheelCenter.x - 18.0f,
        wheelCenter.y - (wheelRadius + 32.0f) * verticalScale};
    const Vector2 pointerRight = {
        wheelCenter.x + 18.0f,
        wheelCenter.y - (wheelRadius + 32.0f) * verticalScale};
    DrawLineEx(
        {wheelCenter.x,
         wheelCenter.y - (wheelRadius + 54.0f) * verticalScale},
        {wheelCenter.x,
         wheelCenter.y - (wheelRadius + 35.0f) * verticalScale},
        9.0f,
        Fade(neonCyan, 0.2f));
    DrawLineEx(
        {wheelCenter.x,
         wheelCenter.y - (wheelRadius + 54.0f) * verticalScale},
        {wheelCenter.x,
         wheelCenter.y - (wheelRadius + 35.0f) * verticalScale},
        2.0f,
        neonCyan);
    DrawTriangle(
        {wheelCenter.x - 24.0f,
         wheelCenter.y - (wheelRadius + 38.0f) * verticalScale},
        {wheelCenter.x + 24.0f,
         wheelCenter.y - (wheelRadius + 38.0f) * verticalScale},
        pointerTip,
        Fade(neonPurple, 0.22f));

    DrawTriangle(pointerLeft, pointerRight, pointerTip, neonCyan);
    DrawLineEx(pointerLeft, pointerTip, 2.0f, StockColors::text);
    DrawLineEx(pointerTip, pointerRight, 2.0f, StockColors::text);
    DrawLineEx(pointerRight, pointerLeft, 2.0f, StockColors::text);

    DrawCircle(wheelCenter.x, wheelCenter.y, 5, BLACK);
}

void RouletteWheel::finishSpin()
{
    rotation = normalizedDegrees(targetRotation);
    spinning = false;

    winningColor = colorForSlot(winningSlot);
    const bool won = betColor == winningColor;
    payoutAmount = won ? wager * (winningSlot == 0 ? 36.0 : 2.0) : 0.0;

    char result[96];
    if (won)
        std::snprintf(result, sizeof(result), "%s - WON $%.2f",
                      colorName(winningColor), payoutAmount);
    else
        std::snprintf(result, sizeof(result), "%s - BET LOST",
                      colorName(winningColor));
    resultText = result;
}

int RouletteWheel::chooseWinningSlot()
{
    return random.integer(0, 36);
}

RouletteColor RouletteWheel::colorForSlot(int slot)
{
    if (slot == 0)
        return RouletteColor::Green;
    return slot % 2 == 1 ? RouletteColor::Red : RouletteColor::Black;
}

const char *RouletteWheel::colorName(RouletteColor color)
{
    switch (color)
    {
    case RouletteColor::Red:
        return "RED";
    case RouletteColor::Black:
        return "BLACK";
    case RouletteColor::Green:
        return "GREEN";
    }

    return "UNKNOWN";
}
