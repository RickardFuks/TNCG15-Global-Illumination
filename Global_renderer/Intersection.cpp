#include "Intersection.h"

#include <cmath>

bool intersectTriangle(
    const Ray& ray,
    const Triangle& triangle,
    double tMin,
    double tMax,
    HitRecord& hit
)
{
    const Vec3 C1 = triangle.v1 - triangle.v0;
    const Vec3 C2 = triangle.v2 - triangle.v0;
    const Vec3 C3 = ray.origin - triangle.v0;

    const Vec3 R1 = cross(ray.direction, C2);
    const Vec3 R2 = cross(C3, C1);

    const double Cs = dot(C1, R1);

	// Check if the ray is parallel to the triangle
    if (std::abs(Cs) < 1e-9)
    {
        return false;
    }
    const double t = dot(C2, R2) / Cs;
    const double u = dot(C3, R1) / Cs;
    const double v = dot(ray.direction, R2) / Cs;

	// Check if the intersection point is inside the triangle
    if (u < 0 || u > 1 || v < 0 || v > 1 || (u + v) > 1)
    { 
		return false;
    }
	// Check if the intersection point is within the valid range of t
    if (t<tMin || t>tMax)
    {
        return false;
    }
	// Update the hit record with the intersection information
	hit.t = t;
	hit.point = ray.at(t);
	hit.material = triangle.material;
	hit.setFaceNormal(ray, triangle.normal);


    return true;
}
bool intersectSphere(
    const Ray& ray,
    const Sphere& sphere,
    double tMin,
    double tMax,
    HitRecord& hit
)
{
    const Vec3 C = sphere.center - ray.origin;

    const double A = dot(ray.direction, C);

    const double B2 = C.lengthSquared() - A * A;
    const double R2 = sphere.radius * sphere.radius;

    if (B2 > R2)
    {
        return false;
    }

    const double tOffset = std::sqrt(R2 - B2);

    const double t1 = A - tOffset;
    const double t2 = A + tOffset;


    double t = 0.0;
	// Check if the intersection points are within the valid range of t
    if (t1 >= tMin && t1 <= tMax)
    {
        t = t1;
    }
    else if (t2 >= tMin && t2 <= tMax)
    {
        t = t2;
    }
    else
    {
        return false;
    }


    hit.t = t;
    hit.point = ray.at(t);
    hit.material = sphere.material;


    const Vec3 outwardNormal = (hit.point - sphere.center).normalized();
    hit.setFaceNormal(ray, outwardNormal);

    return true;
}