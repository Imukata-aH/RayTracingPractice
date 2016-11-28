#pragma once

#include "hitable.h"

class YzRect : public Hitable
{
public:
	YzRect();
	YzRect(float _y0, float _y1, float _z0, float _z1, float _k, material* _mat);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool boundingBox(float t0, float t1, AABB& box) const;

private:
	material* mat;
	float y0, y1, z0, z1, k;
};