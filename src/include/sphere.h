﻿#pragma once

#include "hitable.h"


class Sphere : public Hitable
{
public:
	Sphere(){}
	Sphere(vec3 center, float radius, material* material) : Hitable(material), center(center), radius(radius){};

	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
	virtual bool boundingBox(float t0, float t1, AABB& box) const;

	vec3 getCenter()const { return center; }
	float getRadius()const { return radius; }

private:
	vec3 center;
	float radius;

	//calculate UV coordinate by a hit point on the Sphere
	void getSphereUV(const vec3& p, float& u, float& v) const;
};