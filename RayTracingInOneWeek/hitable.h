#pragma once

#include "ray.h"

struct HitRecord
{
	float t;
	vec3 hitPoint;
	vec3 normal;
};

class Hitable
{
public:
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};