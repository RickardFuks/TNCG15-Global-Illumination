#pragma once

#include "Vec3.h"
#include "Material.h"

struct Triangle
{
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;

    Vec3 normal;
    Material material;

    Triangle(
        const Vec3& v0,
        const Vec3& v1,
        const Vec3& v2,
        const Material& material
    )
        : v0(v0),
          v1(v1),
          v2(v2),
          material(material)
    {
        normal = cross(v1 - v0, v2 - v0).normalized();
    }
};