#pragma once

#include "raylib.h"

#include "colors.h"
#include "../graphics/util.h"
#include "../interface/renderable.h"
#include "../interface/updateable.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

enum class GamePhase
{
    PreMarket,
    Trading,
    AfterHours
};

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

    void render() override
    {
        const std::string timeText = formatTime();
        const std::string phaseText = phaseName();

        GraphicsUtil::drawGlowText(
            timeText,
            {30.0f, 478.0f},
            28,
            StockColors::text,
            StockColors::glow);
        GraphicsUtil::drawGlowText(
            phaseText,
            {30.0f, 510.0f},
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

    float hour() const
    {
        return timeOfDay / secondsPerHour;
    }

    GamePhase phase() const
    {
        if (hour() < marketOpenHour)
            return GamePhase::PreMarket;

        if (hour() < marketCloseHour)
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
        switch (phase())
        {
        case GamePhase::PreMarket:
            return "PRE MARKET";
        case GamePhase::Trading:
            return "TRADING";
        case GamePhase::AfterHours:
            return "AFTER-HOURS";
        }

        return "UNKNOWN";
    }

    static constexpr float secondsPerMinute = 60.0f;
    static constexpr float secondsPerHour = secondsPerMinute * 60.0f;
    static constexpr float secondsPerDay = secondsPerHour * 24.0f;
    static constexpr float marketOpenHour = 6.0f;
    static constexpr float marketCloseHour = 22.0f;
    static constexpr float minimumHourLength = 0.01f;

    float timeOfDay;
    float realSecondsPerGameHour;
    float simulationSpeed = 1.0f;
    float elapsedGameHoursSinceStart = 0.0f;
};
