#include "helperFunctions.h"

Vector2 getCenterOfRec(Rectangle *rec)
{
    return {rec->x + (rec->width / 2), rec->y + (rec->height / 2)};
}
