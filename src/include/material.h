#pragma once

#include "vec3.h"

class ray;
struct HitRecord;

class material
{
public:
	virtual bool scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const = 0;
protected:
	vec3 reflect(const vec3& v, const vec3& n) const
	{
		return v - 2 * dot(v, n) * n;
	}

	// ni: 入射元屈折率 nt: 入射先屈折率
	bool refract(const vec3& v, const vec3& n, float ni, float nt, vec3& refracted) const
	{
		float niOverNt{ ni / nt };
		vec3 uv{ unit_vector(v) };
		float dt{ dot(uv, n) };
		float discriminant{ 1.0f - niOverNt*niOverNt*(1 - dt*dt) };
		if (discriminant > 0) 
		{
			// 屈折した
			refracted = niOverNt*(uv - n*dt) - n * sqrt(discriminant);
			return true;
		}
		else
		{
			// 全反射
			return false;
		}
	}
};