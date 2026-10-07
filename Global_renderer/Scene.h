#pragma once

#include <vector>

#include "Triangle.h"

inline std::vector<Triangle> createRoom()
{
    const Material red(
        MaterialType::Diffuse,
        Vec3(0.8, 0.2, 0.2)
    );

    const Material yellow(
        MaterialType::Diffuse,
        Vec3(0.8, 0.8, 0.2)
    );

    const Material blue(
        MaterialType::Diffuse,
        Vec3(0.2, 0.2, 0.8)
    );

    const Material white(
        MaterialType::Diffuse,
        Vec3(0.8, 0.8, 0.8)
    );
    const Material mirror(
        MaterialType::Mirror,
        Vec3(1.0, 1.0, 1.0)
    );

    std::vector<Triangle> triangles;

    // x = 0, inward normal +x
    triangles.emplace_back(
        Vec3(0, 0, 0), Vec3(0, 4, 0), Vec3(0, 4, 4), white);
    triangles.emplace_back(
        Vec3(0, 0, 0), Vec3(0, 4, 4), Vec3(0, 0, 4), white);

    // x = 4, inward normal -x
    triangles.emplace_back(
        Vec3(4, 0, 0), Vec3(4, 0, 4), Vec3(4, 4, 4), mirror);
    triangles.emplace_back(
        Vec3(4, 0, 0), Vec3(4, 4, 4), Vec3(4, 4, 0), mirror);

    // y = 0, inward normal +y
    triangles.emplace_back(
        Vec3(0, 0, 0), Vec3(0, 0, 4), Vec3(4, 0, 4), red);
    triangles.emplace_back(
        Vec3(0, 0, 0), Vec3(4, 0, 4), Vec3(4, 0, 0), red);

    // y = 4, inward normal -y
    triangles.emplace_back(
        Vec3(0, 4, 0), Vec3(4, 4, 0), Vec3(4, 4, 4), yellow);
    triangles.emplace_back(
        Vec3(0, 4, 0), Vec3(4, 4, 4), Vec3(0, 4, 4), yellow);

    // z = 0, floor, inward normal +z
    triangles.emplace_back(
        Vec3(0, 0, 0), Vec3(4, 0, 0), Vec3(4, 4, 0), blue);
    triangles.emplace_back(
        Vec3(0, 0, 0), Vec3(4, 4, 0), Vec3(0, 4, 0), blue);

    // z = 4, ceiling, inward normal -z
    triangles.emplace_back(
        Vec3(0, 0, 4), Vec3(0, 4, 4), Vec3(4, 4, 4), white);
    triangles.emplace_back(
        Vec3(0, 0, 4), Vec3(4, 4, 4), Vec3(4, 0, 4), white);

    return triangles;
}