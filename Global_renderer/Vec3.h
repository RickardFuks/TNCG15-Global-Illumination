#pragma once

struct Vec3
{
	double x;
	double y;
	double z;


	Vec3() : x(0.0), y(0.0), z(0.0) {
	}
	Vec3(double x, double y, double z) 
		: x(x), y(y), z(z) {
	}
};
