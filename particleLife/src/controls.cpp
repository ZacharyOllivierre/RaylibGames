#include "controls.h"
#include "raylib.h"

void Controls::runControls()
{
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        mouseControls(mousePos);
    }
}

void Controls::mouseControls(const Vector2 &mousePos)
{
    // TODO change to switch
    if (data.clickType == ClickType::Add)
    {
        particleConfig.startingPosition = mousePos;
        int multiSpawnDistance = 5;

        for (int i = 0; i < data.addAmount; i++)
        {
            // TODO update offset code
            particleConfig.startingPosition.x += multiSpawnDistance;
            simulation->addParticle(particleConfig);
        }
    }
    else if (data.clickType == ClickType::Remove)
    {
        simulation->removeParticle(mousePos, data.removeRadius);
    }
    // select
    else
    {
        return;
    }
}