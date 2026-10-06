#pragma once

#include "Vec3.h"

struct Ray
{
    Vec3 origin;
    Vec3 direction;

    Ray()
        : origin(), direction()
    {
    }

    Ray(const Vec3& origin, const Vec3& direction)
        : origin(origin), direction(direction.normalized())
    {
    }

    Vec3 at(double t) const
    {
        return origin + direction * t;
    }
};