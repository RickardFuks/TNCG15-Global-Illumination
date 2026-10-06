#pragma once

#include "Vec3.h"

struct PointLight
{
    Vec3 position;
    Vec3 color;
    double intensity;

    PointLight(
        const Vec3& position,
        const Vec3& color,
        double intensity
    )
        : position(position),
        color(color),
        intensity(intensity)
    {
    }
};