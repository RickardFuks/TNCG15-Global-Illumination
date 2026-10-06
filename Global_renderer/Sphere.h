#pragma once

#include "Vec3.h"
#include "Material.h"

struct Sphere
{
    Vec3 center;
    double radius;
    Material material;

    Sphere(
        const Vec3& center,
        double radius,
        const Material& material
    )
        : center(center),
        radius(radius),
        material(material)
    {
    }
};