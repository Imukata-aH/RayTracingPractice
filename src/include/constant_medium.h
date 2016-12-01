#pragma once

#include "hitable.h"

class Texture;

// The object which has constant density volume.
// This scatter rays probabilistically inside the boundary.
class ConstantMedium : public Hitable
{
public:
	ConstantMedium(Hitable* boundaryObject, float density, Texture* phase);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool boundingBox(float t0, float t1, AABB& box) const;
private:
	Hitable* boundary;
	float density;
	material* phaseFunction;
};