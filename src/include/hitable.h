#pragma once

#include "vec3.h"

class ray;
class material;
class AABB;

struct HitRecord
{
	float t;
	vec3 hitPoint;
	vec3 normal;
	material* material;
	float u;
	float v;
};

class Hitable
{
public:
	Hitable(){}
	Hitable(material* m) : mat(m){}
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const = 0;

	// Create a bounding box.
	// t0[in]: time of beggining of a frame
	// t1[in]: time of end of a frame
	// box[out]: a created bounding box
	virtual bool boundingBox(float t0, float t1, AABB& box) const = 0;
protected:
	material* mat;
};