#pragma once

#include "Ray.h"
#include "Triangle.h"
#include "Sphere.h"
#include "HitRecord.h"

bool intersectTriangle(
    const Ray& ray,
    const Triangle& triangle,
    double tMin,
    double tMax,
    HitRecord& hit
);

bool intersectSphere(
    const Ray& ray,
    const Sphere& sphere,
    double tMin,
    double tMax,
    HitRecord& hit
);