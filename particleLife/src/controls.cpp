#include "controls.h"
#include "raylib.h"

Controls::Controls(Simulation *sim, ParticleConfig startingConfig)
    : simulation(sim), particleConfig(startingConfig)
{
    camera.target = {600, 400};
    camera.offset = {600, 400};
    camera.zoom = 1.0f;
}

void Controls::runControls()
{
    cameraControls();
    numpadControls();

    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        mouseControls(mousePos);
    }
}

// TODO make this work with held clicks
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

void Controls::cameraControls()
{
    float speed = 500.0f * GetFrameTime();

    // move
    if (IsKeyDown(KEY_W))
        camera.target.y -= speed;
    if (IsKeyDown(KEY_S))
        camera.target.y += speed;
    if (IsKeyDown(KEY_A))
        camera.target.x -= speed;
    if (IsKeyDown(KEY_D))
        camera.target.x += speed;

    // zoom
    float zoomAmount = 0.05;
    if (IsKeyDown(KEY_UP))
        camera.zoom += zoomAmount;
    if (IsKeyDown(KEY_DOWN))
        camera.zoom -= zoomAmount;

    // clamp zoom
    if (camera.zoom < 0.1f)
        camera.zoom = 0.1f;

    if (camera.zoom > 5.0f)
        camera.zoom = 5.0f;
}

// numpad number -> click type
void Controls::numpadControls()
{
    // select
    if (IsKeyPressed(KEY_ONE))
    {
        data.clickType = ClickType::Select;
    }
    // add
    else if (IsKeyPressed(KEY_TWO))
    {
        data.clickType = ClickType::Add;
    }
    // remove
    else if (IsKeyPressed(KEY_THREE))
    {
        data.clickType = ClickType::Remove;
    }
}