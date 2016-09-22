#pragma once

#include "hitable.h"

class MovingSphere : public Hitable	//TODO: a property of transition is to be a component, not to be included in a single class
{
public:
	MovingSphere();
	MovingSphere(vec3 cent0, vec3 cent1, float t0, float t1, float r, material *m);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;

	vec3 center(float time) const;
private:
	vec3 center0{}, center1{};
	float time0{}, time1{};
	float radius{};
};