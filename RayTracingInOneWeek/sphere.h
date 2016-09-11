#pragma once

#include "hitable.h"


class Sphere : public Hitable
{
public:
	Sphere(){}
	Sphere(vec3 center, float radius, material* material) : Hitable(material), center(center), radius(radius){};

	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;

	vec3 getCenter()const { return center; }
	float getRadius()const { return radius; }

private:
	vec3 center;
	float radius;
};