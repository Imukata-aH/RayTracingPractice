#pragma once

#include "hitable.h"

// Hitable object that has inverse normal which it wrap
class FlipNormals : public Hitable
{
public:
	FlipNormals(Hitable *object);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool boundingBox(float t0, float t1, AABB& box) const;

private:
	Hitable* obj;
};