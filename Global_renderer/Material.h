#pragma once

#include "Vec3.h"

enum class MaterialType
{
    Diffuse,
    Mirror,
    Transparent,
    Light
};

struct Material
{
    MaterialType type;
    Vec3 color;
    double refractiveIndex;

    Material()
        : type(MaterialType::Diffuse),
        color(1.0, 1.0, 1.0),
        refractiveIndex(1.0)
    {
    }

    Material(
        MaterialType type,
        const Vec3& color,
        double refractiveIndex = 1.0
    )
        : type(type),
        color(color),
        refractiveIndex(refractiveIndex)
    {
    }
};