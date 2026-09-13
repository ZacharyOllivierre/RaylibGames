#include "imguiWindow.h"

void drawImGui(Simulation &simulation, Controls &controls, Graphics &graphics, const Vector2 &worldDimensions)
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

    ImGui::SliderFloat(
        "Density Limit",
        &data.densityLimit,
        0.0f,
        100.0f);
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
    if (ImGui::Button("Zero Matrix"))
    {
        simulation.resetAttraction();
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear Matrix"))
    {
        simulation.clearAttraction();
    }

    ImGui::SameLine();
    if (ImGui::Button("Center All"))
    {
        simulation.centerAll();
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

    // TODO Clean
    ImGui::Separator();
    ImGui::Text("Batch Add");

    static int numParticles;
    int maxTypes = static_cast<int>(SimColors::NUM);
    static int ratio[static_cast<int>(SimColors::NUM)];
    static bool batchAdd;

    if (ImGui::Button("Toggle On"))
    {
        batchAdd = !batchAdd;
    }

    ImGui::SameLine();
    if (ImGui::Button("Spawn"))
    {
        // simulation.clear();

        int ratioSum = arrayTotal(ratio, maxTypes);
        int batchNum = numParticles / ratioSum;

        // TODO bug spawns only in top half on area
        for (int i = 0; i < maxTypes; i++)
        {
            for (int j = 0; j < batchNum * ratio[i]; j++)
            {
                config.typeId = i;
                config.startingPosition.x = GetRandomValue(0, worldDimensions.x);
                config.startingPosition.y = GetRandomValue(0, worldDimensions.y);
                config.startingPosition.y = j;

                simulation.addParticle(config);
            }
        }
    }

    ImGui::SliderInt(
        "Number Particles",
        &numParticles,
        0,
        10000);

    for (int i = 0; i < static_cast<int>(SimColors::NUM) && batchAdd; i++)
    {
        auto s = "Ratio " + simColorToString(static_cast<SimColors>(i));

        ImGui::SliderInt(
            s.c_str(),
            &ratio[i],
            0,
            static_cast<int>(SimColors::NUM));
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
    case SimColors::Violet:
        return "Violet";
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

int arrayTotal(int v[], int size)
{
    int total = 0;
    for (int i = 0; i < size; i++)
    {
        total += v[i];
    }
    return total;
}