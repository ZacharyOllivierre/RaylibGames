#pragma once

#include "interfaces/renderable.h"
#include <vector>

class RenderManager
{
public:
    RenderManager() {}

    void add(Renderable *object)
    {
        objects.push_back(object);
    }

    void render()
    {
        for (Renderable *object : objects)
        {
            object->render();
        }
    }

    void remove(Renderable *object)
    {
        objects.erase(
            std::remove(objects.begin(), objects.end(), object),
            objects.end());
    }

private:
    std::vector<Renderable *> objects;
};