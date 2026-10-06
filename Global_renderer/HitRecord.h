#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"

struct HitRecord
{
    double t;
    Vec3 point;
    Vec3 normal;

    Material material;

    bool frontFace;

    void setFaceNormal(
        const Ray& ray,
        const Vec3& outwardNormal
    )
    {
        frontFace = dot(ray.direction, outwardNormal) < 0.0;

        normal = frontFace
            ? outwardNormal
            : -outwardNormal;
    }
};