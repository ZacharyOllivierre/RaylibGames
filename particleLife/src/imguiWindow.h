#pragma once

#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

#include "graphics.h"
#include "controls.h"
#include "simulation.h"

void drawImGui(Simulation &simulation, Controls &controls, Graphics &graphics, const Vector2 &worldDimensions);

std::string simColorToString(const SimColors color);

ImVec4 ToImGuiColor(Color c);

ImVec4 ToImGuiColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

// TODO definitely doesnt belong here
int arrayTotal(int v[], int size);