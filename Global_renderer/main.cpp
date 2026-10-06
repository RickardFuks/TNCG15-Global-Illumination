#include <iostream>
#include <vector>
#include <algorithm>

#include "PointLight.h"
#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "Triangle.h"
#include "HitRecord.h"
#include "Intersection.h"
#include "Scene.h"
#include "Image.h"
#include "Tetrahedron.h"
int main()
{
    const int resolution = 1000;

    const Vec3 eye(0.5, 2.0, 2.0);

    const double viewPlaneX = 1.0;
    const double delta = 1.0 / resolution;

    std::vector<Triangle> room = createRoom();
    std::vector<Triangle> tetrahedron = createTetrahedron();
	// Create a sphere with center at (3.00, 1.30, 0.61), radius 0.60, and diffuse material with color (0.9, 0.9, 0.9)
    Sphere sphere(
        Vec3(3.00, 1.30, 1.0),
        0.60,
        Material(MaterialType::Diffuse, Vec3(0.9, 0.9, 0.9))
    );

    PointLight light(
        Vec3(2.0, 2.0, 3.8),
        Vec3(1.0, 1.0, 1.0),
		5.0//Change to apply variable intensity
    );

    std::vector<Vec3> pixels(
        resolution * resolution,
        Vec3(0.0, 0.0, 0.0)
    );

    // Loop through every pixel
    for (int j = 0; j < resolution; ++j)
    {
        for (int i = 0; i < resolution; ++i)
        {
            // Center of the current pixel on the view plane
            const double y =
                1.5 + (i + 0.5) * delta;

            const double z =
                2.5 - (j + 0.5) * delta;

            const Vec3 pixelPoint(
                viewPlaneX,
                y,
                z
            );

            // Ray from the eye through this pixel
            Ray ray(
                eye,
                pixelPoint - eye
            );

			// Initialize the closest intersection distance to a large value
            double closestT = 1e30;

            bool hasHit = false;
            HitRecord closestHit;

            Vec3 pixelColor(
                0.0,
                0.0,
                0.0
            );



            // Test ray against every triangle in the room
            for (const Triangle& triangle : room)
            {
                HitRecord hit;

                if (intersectTriangle(
                    ray,
                    triangle,
                    0.001,
                    closestT,
                    hit
                ))
                {
                    closestT = hit.t;
                    closestHit = hit;
                    hasHit = true;
                }
            }
            for (const Triangle& triangle : tetrahedron)
            {
                HitRecord hit;

                if (intersectTriangle(
                    ray,
                    triangle,
                    0.001,
                    closestT,
                    hit
                ))
                {
                    closestT = hit.t;
                    closestHit = hit;
                    hasHit = true;
                }
            }
            {
                HitRecord hit;

                if (intersectSphere(ray, sphere, 0.001, closestT, hit))
                {
                    closestT = hit.t;
                    closestHit = hit;
                    hasHit = true;
                }
            }
            if (hasHit)
            {
                Vec3 toLight =
                    light.position - closestHit.point;

                const double distanceSquared =
                    toLight.lengthSquared();

                const Vec3 lightDirection =
                    toLight.normalized();

                const double cosOmega = std::max(
                    0.0,
                    dot(closestHit.normal, lightDirection)
                );

                const double irradiance =
                    light.intensity
                    * cosOmega
                    / distanceSquared;

                pixelColor =
                    closestHit.material.color
                    * light.color
                    * irradiance;
            }
            // Store the final color for this pixel
            pixels[j * resolution + i] = pixelColor;
        }
    }


    writePPM(
        "render.ppm",
        pixels,
        resolution,
        resolution
    );

    std::cout << "Render complete!\n";

    return 0;
}