#include <iostream>

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"

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

    return 0;
}