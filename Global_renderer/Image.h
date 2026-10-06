#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Vec3.h"

inline void writePPM(
    const std::string& filename,
    const std::vector<Vec3>& pixels,
    int width,
    int height
)
{
    std::ofstream file(filename);

    file << "P3\n";
    file << width << " " << height << "\n";
    file << "255\n";

    for (const Vec3& color : pixels)
    {
        const int r = static_cast<int>(
            255.0 * std::clamp(color.x, 0.0, 1.0));

        const int g = static_cast<int>(
            255.0 * std::clamp(color.y, 0.0, 1.0));

        const int b = static_cast<int>(
            255.0 * std::clamp(color.z, 0.0, 1.0));

        file << r << " " << g << " " << b << "\n";
    }
}