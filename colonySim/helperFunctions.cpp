#include "helperFunctions.h"

Vector2 getCenterOfRec(Rectangle *rec)
{
    return {rec->x + (rec->width / 2), rec->y + (rec->height / 2)};
}

Vector2 randomPointInRec(Rectangle *rec)
{
    Vector2 point;
    point.x = GetRandomValue(rec->x, rec->x + rec->width);
    point.y = GetRandomValue(rec->y, rec->y + rec->height);
    return point;
}
