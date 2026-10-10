#pragma once

#include <vector>

#include "Vec3.h"
#include "Triangle.h"
#include "Material.h"

struct AreaLight
{
    Vec3 center;
    double width;
    double height;

    Vec3 color;
    double radiance;

    AreaLight(
        const Vec3& center_,
        double width_,
        double height_,
        const Vec3& color_,
        double radiance_
    )
        : center(center_),
        width(width_),
        height(height_),
        color(color_),
        radiance(radiance_)
    {
    }

    Vec3 normal() const
    {
        return Vec3(0.0, 0.0, -1.0);
    }

    std::vector<Triangle> createGeometry() const
    {
        const double halfWidth = width * 0.5;
        const double halfHeight = height * 0.5;

        const Vec3 A(
            center.x - halfWidth,
            center.y - halfHeight,
            center.z
        );

        const Vec3 B(
            center.x + halfWidth,
            center.y - halfHeight,
            center.z
        );

        const Vec3 C(
            center.x + halfWidth,
            center.y + halfHeight,
            center.z
        );

        const Vec3 D(
            center.x - halfWidth,
            center.y + halfHeight,
            center.z
        );

        const Material lightMaterial(
            MaterialType::Light,
            color
        );

        return {
            Triangle(A, C, B, lightMaterial),
            Triangle(A, D, C, lightMaterial)
        };
    }
};