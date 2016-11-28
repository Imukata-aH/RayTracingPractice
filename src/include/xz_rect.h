#pragma once

#include "hitable.h"

class XzRect : public Hitable
{
public:
	XzRect();
	XzRect(float _x0, float _x1, float _z0, float _z1, float _k, material* _mat);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool boundingBox(float t0, float t1, AABB& box) const;

private:
	material* mat;
	float x0, x1, z0, z1, k;
};