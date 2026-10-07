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

bool intersectScene(
    const Ray& ray,
    const std::vector<Triangle>& room,
    const std::vector<Triangle>& tetrahedron,
    const Sphere& sphere,
    double tMin,
    double tMax,
    HitRecord& closestHit
)
{
    bool hasHit = false;
    double closestT = tMax;

    for (const Triangle& triangle : room)
    {
        HitRecord hit;

        if (intersectTriangle(ray, triangle, tMin, closestT, hit))
        {
            hasHit = true;
            closestT = hit.t;
            closestHit = hit;
        }
    }

    for (const Triangle& triangle : tetrahedron)
    {
        HitRecord hit;

        if (intersectTriangle(ray, triangle, tMin, closestT, hit))
        {
            hasHit = true;
            closestT = hit.t;
            closestHit = hit;
        }
    }

    {
        HitRecord hit;

        if (intersectSphere(ray, sphere, tMin, closestT, hit))
        {
            hasHit = true;
            closestT = hit.t;
            closestHit = hit;
        }
    }

    return hasHit;
}

bool isInShadow(
    const Vec3& point,
    const Vec3& normal,
    const PointLight& light,
    const std::vector<Triangle>& room,
    const std::vector<Triangle>& tetrahedron,
    const Sphere& sphere
)
{
    const double epsilon = 0.001;

    const Vec3 shadowOrigin = point + normal * epsilon;
    const Vec3 shadowToLight = light.position - shadowOrigin;

    const double distanceToLight = shadowToLight.length();

    Ray shadowRay(
        shadowOrigin,
        shadowToLight
    );

    HitRecord shadowHit;

    if (intersectScene(
        shadowRay,
        room,
        tetrahedron,
        sphere,
        epsilon,
        distanceToLight - epsilon,
        shadowHit
    ))
    {
        return true;
    }

    return false;
}


Vec3 traceRay(
    const Ray& ray,
    const std::vector<Triangle>& room,
    const std::vector<Triangle>& tetrahedron,
    const Sphere& sphere,
    const PointLight& light,
    int depth
)
{
    if (depth <= 0)
    {
        return Vec3(0.0, 0.0, 0.0);
    }

    HitRecord hit;

    if (!intersectScene(
        ray,
        room,
        tetrahedron,
        sphere,
        0.001,
        1e30,
        hit
    ))
    {
        return Vec3(0.0, 0.0, 0.0);
    }

    // Diffuse shading
    if (hit.material.type == MaterialType::Diffuse)
    {
        if (isInShadow(
            hit.point,
            hit.normal,
            light,
            room,
            tetrahedron,
            sphere
        ))
        {
            return Vec3(0.0, 0.0, 0.0);
        }

        const Vec3 toLight = light.position - hit.point;
        const double distanceSquared = toLight.lengthSquared();
        const Vec3 lightDirection = toLight.normalized();

        const double cosOmega = std::max(
            0.0,
            dot(hit.normal, lightDirection)
        );

        const double irradiance =
            light.intensity * cosOmega / distanceSquared;

        return hit.material.color * light.color * irradiance;
    }

    // Perfect mirror
    if (hit.material.type == MaterialType::Mirror)
    {
        const double epsilon = 0.001;

        const Vec3 d = ray.direction;
        const Vec3 N = hit.normal;

        const Vec3 reflectedDirection =
            d - N * (2.0 * dot(d, N));

        Ray reflectedRay(
            hit.point + N * epsilon,
            reflectedDirection
        );

        return traceRay(
            reflectedRay,
            room,
            tetrahedron,
            sphere,
            light,
            depth - 1
        );
    }

    return Vec3(0.0, 0.0, 0.0);
}

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

	// Loop over each pixel in the image
    for (int j = 0; j < resolution; ++j)
    {
        for (int i = 0; i < resolution; ++i)
        {
			// Calculate the y and z coordinates of the pixel in the view plane
            const double y =
                1.5 + (i + 0.5) * delta;

            const double z =
                2.5 - (j + 0.5) * delta;

            const Vec3 pixelPoint(
                viewPlaneX,
                y,
                z
            );
			// Create a ray from the eye to the pixel point
            Ray ray(
                eye,
                pixelPoint - eye
            );
			// Trace the ray and get the color
            Vec3 pixelColor = traceRay(
                ray,
                room,
                tetrahedron,
                sphere,
                light,
                4
            );
			// Store the color in the pixel array
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