#pragma once

#include "hitable.h"
#include "aabb.h"

class RotateY : public Hitable
{
public:
	RotateY(Hitable* originalObject, float angle);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool boundingBox(float t0, float t1, AABB& box) const;

private:
	Hitable* original;
	float sinTheta;
	float cosTheta;
	bool hasBox;
	AABB bbox;
};
