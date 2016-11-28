#pragma once

#include "hitable.h"

class XyRect : public Hitable
{
public:
	XyRect();
	XyRect(float _x0, float _x1, float _y0, float _y1, float _k, material* _mat);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool boundingBox(float t0, float t1, AABB& box) const;

private:
	material* mat;
	float x0, x1, y0, y1, k;
};