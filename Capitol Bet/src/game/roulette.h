#pragma once

#include "colors.h"
#include "random.h"
#include "../interface/renderable.h"
#include "../interface/updateable.h"

#include <string>

enum class RouletteColor
{
    Red,
    Black,
    Green
};

class RouletteWheel : public Renderable, public Updateable
{
public:
    explicit RouletteWheel(RandomSource &random);

    void update(float delta) override;
    void render() override;

    bool startSpin(RouletteColor color, double wager);
    void reset();
    void setVerticalScale(float scale);

    bool isSpinning() const;
    double payout() const;
    const std::string &status() const;

private:
    static constexpr float spinDuration = 5.0f;
    static constexpr float fullCircle = 360.0f;
    static constexpr float pointerAngle = 270.0f;

    void drawWheel() const;
    void finishSpin();
    int chooseWinningSlot();
    static RouletteColor colorForSlot(int slot);
    static const char *colorName(RouletteColor color);

    RandomSource &random;
    RouletteColor betColor = RouletteColor::Red;
    RouletteColor winningColor = RouletteColor::Green;
    double wager = 0.0;
    double payoutAmount = 0.0;
    float rotation = 0.0f;
    float startRotation = 0.0f;
    float targetRotation = 0.0f;
    float spinElapsed = 0.0f;
    float verticalScale = 1.0f;
    int winningSlot = -1;
    bool spinning = false;
    std::string resultText = "Place a bet to spin.";
};
