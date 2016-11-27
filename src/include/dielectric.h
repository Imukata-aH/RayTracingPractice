#pragma once

#include "material.h"

class Dielectric :public material
{
public:
	Dielectric(float ri) : refIdx(ri) {}
	virtual bool scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const;
private:
	float refIdx;
};