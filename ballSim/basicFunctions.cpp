#include "basicFunctions.h"

int getCenterX(Rectangle &rec)
{
    return rec.x + (rec.width / 2);
}

int getCenterY(Rectangle &rec)
{
    return rec.y + (rec.height / 2);
}

Point getCenter(Rectangle &rec)
{
    return {(float)getCenterX(rec), (float)getCenterY(rec)};
}

Color rgbToColor(RGB color)
{
    Color myColor = {color.r, color.g, color.b, color.a};
    return myColor;
}

void dPrintRectangle(Rectangle &rec)
{
    cout << "Rectangle\n";
    cout << "x, y: " << rec.x << ", " << rec.y << endl;
    cout << "Width: " << rec.width << endl;
    cout << "Height: " << rec.height << "\n\n";
}