#pragma once

#include "vec3.h"

class ray;
class material;

struct HitRecord
{
	float t;
	vec3 hitPoint;
	vec3 normal;
	material* material;
};

class Hitable
{
public:
	Hitable(){}
	Hitable(material* m) : mat(m){}
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
protected:
	material* mat;
};