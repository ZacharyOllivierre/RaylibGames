#pragma once

#include <vector>
#include <algorithm>

#include "interfaces/updateable.h"

class UpdateManager
{
public:
    UpdateManager() {}

    void add(Updateable *object)
    {
        if (object == nullptr ||
            std::find(objects.begin(), objects.end(), object) != objects.end())
            return;

        objects.push_back(object);
    }

    void update(float delta)
    {
        for (Updateable *object : objects)
            object->update(delta);
    }

    void remove(Updateable *object)
    {
        objects.erase(
            std::remove(objects.begin(), objects.end(), object),
            objects.end());
    }

private:
    std::vector<Updateable *> objects;
};