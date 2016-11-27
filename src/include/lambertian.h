#pragma once

#include "material.h"
#include "vec3.h"
class Texture;

class Lambertian :public material
{
public:
	Lambertian(Texture* texture);
	virtual bool scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const;
private:
	Texture* albedo;
};