#pragma once

#include "raylib.h"
#include <iostream> // for debugging
using namespace std;

struct Point
{
    float x;
    float y;
};

struct RGB
{
    unsigned char r, g, b, a;
};

// Rectangle helpers
int getCenterX(Rectangle &rec);
int getCenterY(Rectangle &rec);
Point getCenter(Rectangle &rec);

// Color
Color rgbToColor(RGB color);

// Debugging
void dPrintRectangle(Rectangle &rec);
