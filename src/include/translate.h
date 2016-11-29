#pragma once

#include "hitable.h"

// Translate object including this class's instance.
class Translate : public Hitable
{
public:
	Translate(Hitable* orignalObject, const vec3& displacement);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool boundingBox(float t0, float t1, AABB& box) const;

private:
	Hitable* original;
	vec3 offset;
};