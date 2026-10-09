#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

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

    const double exposure = 0.8;

    for (const Vec3& color : pixels)
    {
        // Prevent negative values
        const double rLinear = std::max(0.0, color.x);
        const double gLinear = std::max(0.0, color.y);
        const double bLinear = std::max(0.0, color.z);

        // Simple exponential tone mapping
        double r = 1.0 - std::exp(-rLinear * exposure);
        double g = 1.0 - std::exp(-gLinear * exposure);
        double b = 1.0 - std::exp(-bLinear * exposure);

        // Gamma correction, gamma ~= 0.5
        r = std::sqrt(r);
        g = std::sqrt(g);
        b = std::sqrt(b);

        const int ir = static_cast<int>(
            255.0 * std::clamp(r, 0.0, 1.0)
            );

        const int ig = static_cast<int>(
            255.0 * std::clamp(g, 0.0, 1.0)
            );

        const int ib = static_cast<int>(
            255.0 * std::clamp(b, 0.0, 1.0)
            );

        file << ir << " " << ig << " " << ib << "\n";
    }
}