#pragma once

#include "material.h"
#include "texture.h"
#include "ray.h"
#include "hitable.h"
#include "random_util.h"

// scatter rays in random directions
class Isotropic : public material
{
public:
	Isotropic(Texture* albedo) : albedo(albedo) {};
	virtual bool scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const
	{
		// scattering the ray in random directions
		scattered = ray{ rec.hitPoint, RandomUtil::getrRandomPointInUnitSphere()};
		attenuation = albedo->value(rec.u, rec.v, rec.hitPoint);
		return true;
	}
private:
	Texture* albedo;
};