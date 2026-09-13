#include "raylib.h"

#include "VulkanCompute.h"

#include "simulationTypes.h"
#include "simulation.h"
#include "controls.h"
#include "graphics.h"

#include "imguiWindow.h"

Vector2 screenDimensions = {1200, 800};
Vector2 worldDimensions = {5000, 5000};

int main()
{
    InitWindow(screenDimensions.x, screenDimensions.y, "Particle Life");
    SetTargetFPS(60);

    rlImGuiSetup(true);

    // TODO gross make presets
    SimData data = {50, -1, 200, 0.75, 20, 0.8, 49, worldDimensions, 0};
    Simulation simulation(data);

    ParticleConfig config = {0, 5, {0, 0}, {0, 0}};
    Controls controls(&simulation, config);

    Graphics graphics(simulation.getParticles());

    // Vulcan test
    VulkanCompute vulkan;
    vulkan.initialize(simulation);
    vulkan.uploadParticles(simulation.getParticles());
    vulkan.dispatch(simulation.getParticles().size());
    vulkan.readbackParticles(simulation.getParticles());

    while (!WindowShouldClose())
    {
        // Controls
        controls.runControls();

        // Vulcan
        vulkan.uploadSimulation(simulation);
        vulkan.uploadParticles(simulation.getParticles());
        vulkan.dispatch(simulation.getParticles().size());
        vulkan.readbackParticles(simulation.getParticles());

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(controls.getCamera());
        graphics.draw();
        EndMode2D();

        DrawFPS(10, 10);

        rlImGuiBegin();
        drawImGui(simulation, controls, graphics, worldDimensions);
        rlImGuiEnd();

        EndDrawing();

        // Simulation
        // simulation.updateParticles();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
