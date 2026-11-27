#pragma once

#include "raylib.h"

#include "colors.h"
#include "../graphics/util.h"
#include "../interface/renderable.h"
#include "../interface/updateable.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <string>

enum class GamePhase
{
    Casino,
    Trading,
    AfterHours
};

inline const char *gamePhaseName(GamePhase phase)
{
    switch (phase)
    {
    case GamePhase::Casino:
        return "CASINO";
    case GamePhase::Trading:
        return "TRADING";
    case GamePhase::AfterHours:
        return "AFTER-HOURS";
    }

    return "UNKNOWN";
}

class GameClock : public Renderable, public Updateable
{
public:
    GameClock(float startingHour, float realSecondsPerGameHour)
        : timeOfDay(startingHour * secondsPerHour),
          realSecondsPerGameHour(
              std::max(realSecondsPerGameHour, minimumHourLength))
    {
    }

    void update(float delta) override
    {
        const float gameHoursElapsed =
            delta * simulationSpeed / realSecondsPerGameHour;
        elapsedGameHoursSinceStart += gameHoursElapsed;
        timeOfDay += gameHoursElapsed * secondsPerHour;
        timeOfDay = std::fmod(timeOfDay, secondsPerDay);

        if (timeOfDay < 0.0f)
            timeOfDay += secondsPerDay;
    }

    void reset()
    {
        timeOfDay = tradingOpenHour * secondsPerHour;
        simulationSpeed = 1.0f;
        elapsedGameHoursSinceStart = 0.0f;
    }

    bool advanceToNextTime(int hour, int minute)
    {
        if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60)
            return false;

        const float targetTime =
            static_cast<float>(hour * 60 + minute) * secondsPerMinute;
        float elapsedSeconds = targetTime - timeOfDay;
        if (elapsedSeconds <= 0.0f)
            elapsedSeconds += secondsPerDay;

        timeOfDay = targetTime;
        elapsedGameHoursSinceStart += elapsedSeconds / secondsPerHour;
        return true;
    }

    void render() override
    {
        const std::string timeText = formatTime();
        const std::string phaseText = phaseName();

        GraphicsUtil::drawGlowText(
            timeText,
            {30.0f, 384.0f},
            28,
            StockColors::text,
            StockColors::glow);
        GraphicsUtil::drawGlowText(
            phaseText,
            {30.0f, 416.0f},
            16,
            StockColors::mutedText,
            StockColors::glow);
    }

    void setSimulationSpeed(float speed)
    {
        simulationSpeed = std::max(speed, 0.0f);
    }

    void changeSimulationSpeed(float amount)
    {
        setSimulationSpeed(simulationSpeed + amount);
    }

    float speed() const
    {
        return simulationSpeed;
    }

    float elapsedGameHours() const
    {
        return elapsedGameHoursSinceStart;
    }

    std::uint64_t tradingDayIndex() const
    {
        return static_cast<std::uint64_t>(
            elapsedGameHoursSinceStart / 24.0f);
    }

    float hour() const
    {
        return timeOfDay / secondsPerHour;
    }

    GamePhase phase() const
    {
        const float currentHour = hour();
        if (currentHour < tradingOpenHour || currentHour >= casinoOpenHour)
            return GamePhase::Casino;

        if (currentHour < afterHoursOpenHour)
            return GamePhase::Trading;

        return GamePhase::AfterHours;
    }

    std::string formatTime() const
    {
        const int totalMinutes =
            static_cast<int>(timeOfDay / secondsPerMinute);
        const int hours = (totalMinutes / 60) % 24;
        const int minutes = totalMinutes % 60;
        char buffer[16];
        std::snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, minutes);
        return buffer;
    }

private:
    std::string phaseName() const
    {
        return gamePhaseName(phase());
    }

    static constexpr float secondsPerMinute = 60.0f;
    static constexpr float secondsPerHour = secondsPerMinute * 60.0f;
    static constexpr float secondsPerDay = secondsPerHour * 24.0f;
    static constexpr float tradingOpenHour = 6.0f;
    static constexpr float afterHoursOpenHour = 18.0f;
    static constexpr float casinoOpenHour = 22.0f;
    static constexpr float minimumHourLength = 0.01f;

    float timeOfDay;
    float realSecondsPerGameHour;
    float simulationSpeed = 1.0f;
    float elapsedGameHoursSinceStart = 0.0f;
};
