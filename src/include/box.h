#pragma once

#include "hitable.h"

class vec3;

class Box : public Hitable {
public:
	Box();
	Box(const vec3& p0, const vec3& p1, material* mat);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool boundingBox(float t0, float t1, AABB& box) const;

private:
	vec3 pMin, pMax;
	Hitable* hitableListPtr;
};