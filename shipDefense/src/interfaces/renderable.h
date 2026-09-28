#pragma once

class Renderable
{
public:
    virtual ~Renderable() = default;

    virtual void render() = 0;
};