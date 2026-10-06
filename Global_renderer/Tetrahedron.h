#pragma once

#include <vector>

#include "Triangle.h"

inline std::vector<Triangle> createTetrahedron()
{
    const Material material(
        MaterialType::Diffuse,
        Vec3(0.9, 0.2, 0.8)
    );

    const Vec3 A(3.80, 1.75, 0.01);
    const Vec3 B(3.80, 3.25, 0.01);
    const Vec3 C(2.50, 2.50, 0.01);
    const Vec3 D(3.37, 2.50, 1.23);

    std::vector<Triangle> triangles;

    // Base
    triangles.emplace_back(C, B, A, material);

    // Sides
    triangles.emplace_back(A, B, D, material);
    triangles.emplace_back(B, C, D, material);
    triangles.emplace_back(C, A, D, material);

    return triangles;
}
