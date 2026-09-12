#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

#include "VulkanCompute.h"

#include "simulationTypes.h"
#include "simulation.h"
#include "controls.h"
#include "graphics.h"

Vector2 screenDimensions = {1200, 800};
Vector2 worldDimensions = {3000, 3000};

// TODO move to imgui place
void drawImGui(Simulation &simulation, Controls &controls, Graphics &graphics);
std::string simColorToString(const SimColors color);
ImVec4 ToImGuiColor(Color c);
ImVec4 ToImGuiColor(unsigned char r, unsigned char g,
                    unsigned char b, unsigned char a = 255);

int main()
{
    InitWindow(screenDimensions.x, screenDimensions.y, "Particle Life");
    SetTargetFPS(60);

    rlImGuiSetup(true);

    // TODO gross make presets
    SimData data = {5, -1, 200, 1, 50, 0.4, worldDimensions, 0};
    Simulation simulation(data);

    ParticleConfig config = {1, 5, {0, 0}, {0, 0}};
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

        // Vulcan test
        vulkan.uploadSimulation(simulation);
        vulkan.uploadParticles(simulation.getParticles());
        vulkan.dispatch(simulation.getParticles().size());
        vulkan.readbackParticles(simulation.getParticles());
        // End of Vulkan test

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(controls.getCamera());
        graphics.draw();
        EndMode2D();

        DrawFPS(10, 10);

        rlImGuiBegin();
        drawImGui(simulation, controls, graphics);
        rlImGuiEnd();

        EndDrawing();

        // Simulation
        // simulation.updateParticles();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}

void drawImGui(Simulation &simulation, Controls &controls, Graphics &graphics)
{
    SimData &data = simulation.getData();
    ParticleConfig &config = controls.getParticleConfig();
    ControlData &controlData = controls.getControlData();

    ImGui::Begin("Particle Life");

    // Simulation controls
    // avoid infinite attraction wrapping (-1 just to be safe)
    static float smaller = fmin(worldDimensions.x, worldDimensions.y);
    ImGui::Text("Simulation");
    ImGui::SliderFloat(
        "Innate Repulsion Area",
        &data.innateRepulsionArea,
        0.0f,
        smaller / 2 - 1);

    ImGui::SliderFloat(
        "Innate Repulsion Amount",
        &data.innateRepulsion,
        -20.0f,
        0.0f);
    ImGui::SliderFloat(
        "Max Attraction Area",
        &data.maxAttractionArea,
        0.0f,
        smaller / 2 - 1);

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
    const char *clickTypes[] = {"Select", "Add", "Remove"};
    int currentType = static_cast<int>(controlData.clickType);

    if (ImGui::Combo("Click Type", &currentType, clickTypes, IM_ARRAYSIZE(clickTypes)))
    {
        controlData.clickType = static_cast<ClickType>(currentType);
    }

    ImGui::SliderInt(
        "Add Amount",
        &controlData.addAmount,
        1,
        500);

    ImGui::SliderFloat(
        "Particle Size",
        &config.size,
        1.0f,
        30.0f);

    ImGui::SliderInt(
        "Type",
        &config.typeId,
        0,
        static_cast<int>(SimColors::NUM) - 1);

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

    ImGui::SliderInt(
        "Remove Radius",
        &controlData.removeRadius,
        1,
        100);
    ImGui::Separator();

    // Interaction matrix
    ImGui::Text("Interaction Matrix");
    if (ImGui::BeginTable("InteractionMatrix", data.numTypes + 1))
    {
        // header row
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("From / To");

        ImGui::PushStyleColor(ImGuiCol_Text, ToImGuiColor(0, 0, 0));
        // column headers
        for (int j = 0; j < data.numTypes; j++)
        {
            ImGui::TableSetColumnIndex(j + 1);
            ImGui::Text("%s", simColorToString(static_cast<SimColors>(j)).c_str());

            // background color
            ImGui::TableSetBgColor(
                ImGuiTableBgTarget_CellBg,
                ImGui::ColorConvertFloat4ToU32(ToImGuiColor(graphics.getParticleColor(j))));
        }
        ImGui::PopStyleColor();

        // Matrix
        for (int i = 0; i < data.numTypes; i++)
        {
            ImGui::TableNextRow();

            // row header
            ImGui::PushStyleColor(ImGuiCol_Text, ToImGuiColor(0, 0, 0));
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", simColorToString(static_cast<SimColors>(i)).c_str());
            ImGui::TableSetBgColor(
                ImGuiTableBgTarget_CellBg,
                ImGui::ColorConvertFloat4ToU32(ToImGuiColor(graphics.getParticleColor(i))));
            ImGui::PopStyleColor();

            // values
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

    ImGui::SameLine();
    if (ImGui::Button("Randomize Pos"))
    {
        simulation.randomizePositions();
    }

    ImGui::Separator();
    ImGui::Text("Particle Size Updater thing");
    ImGui::Text("Updates with particle cursor size");

    static int id;
    ImGui::SliderInt(
        "ID to update",
        &id,
        0,
        data.numTypes - 1);

    ImGui::SameLine();
    if (ImGui::Button("Send"))
    {
        simulation.updateParticleSize(id, config.size);
    }

    ImGui::End();
}

// goes with imgui
std::string simColorToString(const SimColors color)
{
    switch (color)
    {
    case SimColors::Red:
        return "Red";
    case SimColors::Green:
        return "Green";
    case SimColors::Blue:
        return "Blue";
    case SimColors::Purple:
        return "Purple";
    case SimColors::Yellow:
        return "Yellow";
    case SimColors::Orange:
        return "Orange";
    case SimColors::Pink:
        return "Pink";
    case SimColors::White:
        return "White";
    case SimColors::Lime:
        return "Lime";
    case SimColors::Skyblue:
        return "Skyblue";
    case SimColors::Gray:
        return "Grey";
    case SimColors::Brown:
        return "Brown";
    default:
        return "error";
    }
}

ImVec4 ToImGuiColor(Color c)
{
    return ImVec4(
        c.r / 255.0f,
        c.g / 255.0f,
        c.b / 255.0f,
        c.a / 255.0f);
}

ImVec4 ToImGuiColor(unsigned char r, unsigned char g,
                    unsigned char b, unsigned char a)
{
    return ImVec4(
        r / 255.0f,
        g / 255.0f,
        b / 255.0f,
        a / 255.0f);
}