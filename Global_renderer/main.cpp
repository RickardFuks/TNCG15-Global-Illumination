#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <chrono>

//#include "PointLight.h"
#include "AreaLight.h"
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
    const std::vector<Triangle>& lightGeometry,
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

    for (const Triangle& triangle : lightGeometry)
    {
        HitRecord hit;

        if (intersectTriangle(
            ray,
            triangle,
            tMin,
            closestT,
            hit
        ))
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
    const Vec3& lightPoint,
    const std::vector<Triangle>& room,
    const std::vector<Triangle>& tetrahedron,
    const Sphere& sphere
)
{
    const double epsilon = 0.001;

    const Vec3 shadowOrigin =
        point + normal * epsilon;

    const Vec3 shadowToLight =
        lightPoint - shadowOrigin;

    const double distanceToLight =
        shadowToLight.length();

    Ray shadowRay(
        shadowOrigin,
        shadowToLight
    );

    for (const Triangle& triangle : room)
    {
        HitRecord shadowHit;

        if (intersectTriangle(
            shadowRay,
            triangle,
            epsilon,
            distanceToLight - epsilon,
            shadowHit
        ))
        {
            return true;
        }
    }

    for (const Triangle& triangle : tetrahedron)
    {
        HitRecord shadowHit;

        if (intersectTriangle(
            shadowRay,
            triangle,
            epsilon,
            distanceToLight - epsilon,
            shadowHit
        ))
        {
            return true;
        }
    }

    if (sphere.material.type != MaterialType::Transparent)
    {
        HitRecord shadowHit;

        if (intersectSphere(
            shadowRay,
            sphere,
            epsilon,
            distanceToLight - epsilon,
            shadowHit
        ))
        {
            return true;
        }
    }

    return false;
}

Vec3 reflectDirection(
    const Vec3& d,
    const Vec3& N
)
{
    return d - N * (2.0 * dot(d, N));
}

double schlickReflectance(
    double cosTheta,
    double n1,
    double n2
)
{
    double r0 = (n1 - n2) / (n1 + n2);
    r0 = r0 * r0;

    return r0
        + (1.0 - r0)
        * std::pow(1.0 - cosTheta, 5.0);
}

double randomDouble()
{
    static std::mt19937 generator(
        std::random_device{}()
    );

    static std::uniform_real_distribution<double> distribution(
        0.0,
        1.0
    );

    return distribution(generator);
}

Vec3 refractDirection(
    const Vec3& d,
    const Vec3& N,
    double eta
)
{
    const double cosTheta =
        std::min(dot(-d, N), 1.0);

    const double sinThetaSquared =
        std::max(
            0.0,
            1.0 - cosTheta * cosTheta
        );

    const double cosThetaT =
        std::sqrt(
            1.0
            - eta * eta * sinThetaSquared
        );

    return
        d * eta
        + N * (eta * cosTheta - cosThetaT);
}
// Generate a random direction in the hemisphere around the normal N
Vec3 randomCosineDirection(const Vec3& N)
{
    const double x = randomDouble();
    const double y = randomDouble();

    const double phi = 2.0 * 3.14159265358979323846 * x;

    const double localX =
        std::cos(phi) * std::sqrt(1.0 - y);

    const double localY =
        std::sin(phi) * std::sqrt(1.0 - y);

    const double localZ =
        std::sqrt(y);

    Vec3 helper;

    if (std::abs(N.z) < 0.999)
    {
        helper = Vec3(0.0, 0.0, 1.0);
    }
    else
    {
        helper = Vec3(1.0, 0.0, 0.0);
    }

    const Vec3 T =
        cross(helper, N).normalized();

    const Vec3 B =
        cross(N, T);

    return (
        T * localX
        + B * localY
        + N * localZ
        ).normalized();
}
Vec3 sampleAreaLight(const AreaLight& light)
{
    const double x =
        light.center.x
        + (randomDouble() - 0.5) * light.width;

    const double y =
        light.center.y
        + (randomDouble() - 0.5) * light.height;

    return Vec3(
        x,
        y,
        light.center.z
    );
}

//Mucho important 
Vec3 traceRay(
    const Ray& ray,
    const std::vector<Triangle>& room,
    const std::vector<Triangle>& tetrahedron,
    const std::vector<Triangle>& lightGeometry,
    const Sphere& sphere,
    const AreaLight& light,
    int depth,
    bool useMonteCarloDiffuse,
    bool previousWasSpecular
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
        lightGeometry,
        sphere,
        0.001,
        1e30,
        hit
    ))
    {
        return Vec3(0.0, 0.0, 0.0);
    }
    if (hit.material.type == MaterialType::Light)
    {

    return light.color * light.radiance;
    }
    // Diffuse shading
    if (hit.material.type == MaterialType::Diffuse)
    {
        Vec3 directLight(0.0, 0.0, 0.0);

        const Vec3 lightPoint =
            sampleAreaLight(light);

        if (!isInShadow(
            hit.point,
            hit.normal,
            lightPoint,
            room,
            tetrahedron,
            sphere
        ))
        {
            const Vec3 toLight =
                lightPoint - hit.point;

            const double distanceSquared =
                toLight.lengthSquared();

            const Vec3 lightDirection =
                toLight.normalized();

            const double cosSurface =
                std::max(
                    0.0,
                    dot(hit.normal, lightDirection)
                );

            const Vec3 lightNormal =
                light.normal();

            const double cosLight =
                std::max(
                    0.0,
                    dot(lightNormal, -lightDirection)
                );

            const double geometry =
                cosSurface * cosLight
                / distanceSquared;

            const double lightArea =
                light.width * light.height;

            const double pi =
                3.141592;

            directLight =
                hit.material.color
                * light.color
                * light.radiance
                * geometry
                * lightArea
                / pi;
        }

        if (!useMonteCarloDiffuse)
        {
            return directLight;
        }

        const double rho = std::clamp(
            std::max({
                hit.material.color.x,
                hit.material.color.y,
                hit.material.color.z
                }),
            0.0,
            1.0
        );

        if (depth <= 1 || rho <= 0.0 || randomDouble() > rho)
        {
            return directLight;
        }

        const Vec3 diffuseDirection =
            randomCosineDirection(hit.normal);

        const double epsilon = 0.001;

        Ray diffuseRay(
            hit.point + hit.normal * epsilon,
            diffuseDirection
        );

        const Vec3 indirectLight = traceRay(
            diffuseRay,
            room,
            tetrahedron,
            lightGeometry,
            sphere,
            light,
            depth - 1,
            useMonteCarloDiffuse,
            false
        );
		// Combine direct and indirect light, scaling the indirect light by the material color and rho
		//Russian roulette termination: scale the indirect light by the material color and rho to account for the probability of continuing the path
        return directLight + (hit.material.color / rho) * indirectLight;
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
            lightGeometry,
            sphere,
            light,
            depth - 1,
            useMonteCarloDiffuse,
            true
        );
    }
    if (hit.material.type == MaterialType::Transparent)
    {
        const double epsilon = 0.001;

        const double nGlass =
            hit.material.refractiveIndex;

        double n1;
        double n2;

        if (hit.frontFace)
        {
            // Air -> glass
            n1 = 1.0;
            n2 = nGlass;
        }
        else
        {
            // Glass -> air
            n1 = nGlass;
            n2 = 1.0;
        }

        const double eta = n1 / n2;

        const double cosTheta =
            std::min(
                dot(-ray.direction, hit.normal),
                1.0
            );

        const double sinTheta =
            std::sqrt(
                std::max(
                    0.0,
                    1.0 - cosTheta * cosTheta
                )
            );

        const bool totalInternalReflection =
            eta * sinTheta > 1.0;

        const double reflectionProbability =
            schlickReflectance(
                cosTheta,
                n1,
                n2
            );

        Vec3 newDirection;
        Vec3 newOrigin;

        if (
            totalInternalReflection
            || randomDouble() <= reflectionProbability
            )
        {
            // Reflection
            newDirection =
                reflectDirection(
                    ray.direction,
                    hit.normal
                );

            newOrigin =
                hit.point
                + hit.normal * epsilon;
        }
        else
        {
            // Transmission / refraction
            newDirection =
                refractDirection(
                    ray.direction,
                    hit.normal,
                    eta
                );

            newOrigin =
                hit.point
                - hit.normal * epsilon;
        }

        Ray childRay(
            newOrigin,
            newDirection
        );

        return traceRay(
            childRay,
            room,
            tetrahedron,
            lightGeometry,
            sphere,
            light,
            depth - 1,
            useMonteCarloDiffuse,
            true
        );
    }

    return Vec3(0.0, 0.0, 0.0);
}



int main()
{
	const int resolution = 200; //Change to apply variable resolution

    const Vec3 eye(0.5, 2.0, 2.0);

    const bool useMonteCarloDiffuse = true;
    const double viewPlaneX = 1.0;
    const double delta = 1.0 / resolution;

    std::vector<Triangle> room = createRoom();
    std::vector<Triangle> tetrahedron = createTetrahedron();
	
    Sphere sphere(
        Vec3(3.00, 1.30, 1.0),
        0.60,
        Material(
            MaterialType::Transparent,
            Vec3(1.0, 1.0, 1.0),
			1.5//Change to apply variable refractive index
        )
    );

    AreaLight light(
        Vec3(2.0, 2.0, 3.99),
        1.0,
        1.0,
        Vec3(1.0, 1.0, 1.0),
		20.0 //Change to apply variable light intensity
    );
    std::vector<Triangle> lightGeometry =
        light.createGeometry();

    std::vector<Vec3> pixels(
        resolution * resolution,
        Vec3(0.0, 0.0, 0.0)
    );

	const int samplesPerPixel = 100; //Change to apply variable samples per pixel
    const auto startTime = std::chrono::high_resolution_clock::now();

    for (int j = 0; j < resolution; ++j)
    {
        for (int i = 0; i < resolution; ++i)
        {
            Vec3 pixelColor(0.0, 0.0, 0.0);

            for (int sample = 0; sample < samplesPerPixel; ++sample)
            {
                const double y =
                    1.5 + (i + randomDouble()) * delta;

                const double z =
                    2.5 - (j + randomDouble()) * delta;

                const Vec3 pixelPoint(
                    viewPlaneX,
                    y,
                    z
                );

                Ray ray(
                    eye,
                    pixelPoint - eye
                );

                pixelColor = pixelColor + traceRay(
                    ray,
                    room,
                    tetrahedron,
                    lightGeometry,
                    sphere,
                    light,
                    6,
                    useMonteCarloDiffuse,
                    true
                );
            }

            pixelColor = pixelColor / samplesPerPixel;

            pixels[j * resolution + i] = pixelColor;
        }
    }
    const auto endTime =
        std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double> renderTime =
        endTime - startTime;

    std::cout
        << "Render time: "
        << renderTime.count()
        << " seconds\n";


    writePPM(
        "render.ppm",
        pixels,
        resolution,
        resolution
    );

    std::cout << "Render complete!\n";

    return 0;
}