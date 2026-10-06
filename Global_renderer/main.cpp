#include <iostream>

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "Sphere.h"
#include "Intersection.h"

int main()
{
    Vec3 eye(0.5, 2.0, 2.0);
    Vec3 pixel(1.0, 2.0, 2.0);

    Vec3 direction = (pixel - eye).normalized();

    Ray ray(eye, direction);

    Material wall(
        MaterialType::Diffuse,
        Vec3(0.8, 0.2, 0.2)

    );
 
    std::cout
        << ray.direction.x << " "
        << ray.direction.y << " "
        << ray.direction.z << "\n";

    Material red(
        MaterialType::Diffuse,
        Vec3(1.0, 0.0, 0.0)
    );

    Sphere testSphere(
        Vec3(5.0, 0.0, 0.0),
        1.0,
        red
    );

    Ray testRay(
        Vec3(0.0, 0.0, 0.0),
        Vec3(1.0, 0.0, 0.0)
    );

    HitRecord hit;

    if (intersectSphere(testRay, testSphere, 0.001, 1000.0, hit))
    {
        std::cout << "Sphere hit!\n";
        std::cout << "t = " << hit.t << "\n";
        std::cout << "Point = "
            << hit.point.x << " "
            << hit.point.y << " "
            << hit.point.z << "\n";
    }
    else
    {
        std::cout << "Sphere missed!\n";
    }
    return 0;
}