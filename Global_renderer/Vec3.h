#pragma once

#include <cmath>

struct Vec3
{
    double x;
    double y;
    double z;

    Vec3()
        : x(0.0), y(0.0), z(0.0)
    {
    }

    Vec3(double x, double y, double z)
        : x(x), y(y), z(z)
    {
    }

    Vec3 operator+(const Vec3& other) const
    {
        return Vec3(
            x + other.x,
            y + other.y,
            z + other.z
        );
    }

    Vec3 operator-(const Vec3& other) const
    {
        return Vec3(
            x - other.x,
            y - other.y,
            z - other.z
        );
    }

    Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }

    Vec3 operator*(const Vec3& other) const
    {
        return Vec3(
            x * other.x,
            y * other.y,
            z * other.z
        );
    }
    Vec3 operator*(double scalar) const
    {
        return Vec3(
            x * scalar,
            y * scalar,
            z * scalar
        );
    }

    Vec3 operator/(double scalar) const
    {
        return Vec3(
            x / scalar,
            y / scalar,
            z / scalar
        );
    }

    double lengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    double length() const
    {
        return std::sqrt(lengthSquared());
    }

    Vec3 normalized() const
    {
        double len = length();

        if (len == 0.0)
        {
            return Vec3();
        }

        return *this / len;
    }
};

inline double dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x
        + a.y * b.y
        + a.z * b.z;
}

inline Vec3 cross(const Vec3& a, const Vec3& b)
{
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

inline Vec3 operator*(double scalar, const Vec3& vector)
{
    return vector * scalar;
}