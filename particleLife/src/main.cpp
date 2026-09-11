#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

#include "simulationTypes.h"
#include "simulation.h"
#include "controls.h"
#include "graphics.h"

Vector2 screenDimensions = {1200, 800};

void drawImGui(Simulation &simulation, Controls &controls);

// TODO IMPORTANT "cells" explode on walls becasuse attraction isnt calculated
// accross world borders
int main()
{
    InitWindow(screenDimensions.x, screenDimensions.y, "Particle Life");
    SetTargetFPS(60);

    rlImGuiSetup(true);

    // TODO gross make presets
    SimData data = {5, -1, 200, 1, 50, 0.4, screenDimensions, 0};
    Simulation simulation(data);

    ParticleConfig config = {1, 5, {0, 0}, {0, 0}};
    Controls controls(&simulation, config);

    Graphics graphics(simulation.getParticles());

    while (!WindowShouldClose())
    {
        // Controls
        controls.runControls();

        // Drawing
        BeginDrawing();
        ClearBackground(BLUE);
        DrawFPS(10, 10);

        graphics.draw();

        rlImGuiBegin();
        drawImGui(simulation, controls);
        rlImGuiEnd();

        EndDrawing();

        // Simulation
        simulation.updateParticles();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}

void drawImGui(Simulation &simulation, Controls &controls)
{
    SimData &data = simulation.getData();
    ParticleConfig &config = controls.getParticleConfig();
    ControlData &controlData = controls.getControlData();

    ImGui::Begin("Particle Life");

    // Simulation controls
    ImGui::Text("Simulation");

    ImGui::SliderFloat(
        "Particle Size",
        &config.size,
        1.0f,
        20.0f);

    ImGui::SliderFloat(
        "Innate Repulsion Area",
        &data.innateRepulsionArea,
        0.0f,
        200.0f);

    ImGui::SliderFloat(
        "Innate Repulsion Amount",
        &data.innateRepulsion,
        -20.0f,
        0.0f);

    ImGui::SliderFloat(
        "Max Attraction Area",
        &data.maxAttractionArea,
        0.0f,
        500.0f);

    ImGui::SliderFloat(
        "Max Attraction",
        &data.maxAttraction,
        -1.0f,
        1.0f);

    ImGui::SliderFloat(
        "Max Speed",
        &data.maxSpeed,
        0.0f,
        500.0f);

    ImGui::SliderFloat(
        "Damping",
        &data.damping,
        0.0f,
        1.0f);

    ImGui::Separator();

    // Particle cursor
    const char *clickTypes[] = {"Add", "Remove", "Select"};
    int currentType = static_cast<int>(controlData.clickType);

    if (ImGui::Combo("Click Type", &currentType, clickTypes, IM_ARRAYSIZE(clickTypes)))
    {
        controlData.clickType = static_cast<ClickType>(currentType);
    }

    ImGui::SliderInt(
        "Add Amount",
        &controlData.addAmount,
        1,
        20);

    ImGui::SliderInt(
        "Remove Radius",
        &controlData.removeRadius,
        1,
        100);

    ImGui::SliderInt(
        "Type",
        &config.typeId,
        0,
        10);

    ImGui::SliderFloat(
        "Starting Velocity X",
        &config.startingVelocity.x,
        -100.0f,
        100.0f);

    ImGui::SliderFloat(
        "Starting Velocity Y",
        &config.startingVelocity.y,
        -100.0f,
        100.0f);

    ImGui::Separator();

    // Interaction matrix
    ImGui::Text("Interaction Matrix");
    if (ImGui::BeginTable("InteractionMatrix", data.numTypes + 1))
    {
        // Header row
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("From / To");

        for (int j = 0; j < data.numTypes; j++)
        {
            ImGui::TableSetColumnIndex(j + 1);
            ImGui::Text("%d", j);
        }

        // Matrix
        for (int i = 0; i < data.numTypes; i++)
        {
            ImGui::TableNextRow();

            // Row header
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", i);

            // Values
            for (int j = 0; j < data.numTypes; j++)
            {
                ImGui::TableSetColumnIndex(j + 1);

                ImGui::PushID(i * data.numTypes + j);

                ImGui::SetNextItemWidth(70.0f);

                ImGui::DragFloat(
                    "##value",
                    &data.attraction[i][j],
                    0.01f,
                    -1.0f,
                    1.0f);

                ImGui::PopID();
            }
        }

        ImGui::EndTable();
    }

    ImGui::Separator();

    // Resets
    if (ImGui::Button("Reset Board"))
    {
        simulation.clear();
    }
    ImGui::SameLine();

    if (ImGui::Button("Reset Matrix"))
    {
        simulation.resetAttraction();

        // may need if reseting maxtrix causes seg fault
        // simulation.clear();
    }

    ImGui::End();
}