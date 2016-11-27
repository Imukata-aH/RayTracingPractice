#pragma once

#include "material.h"

class Texture;

class DiffuseLight : public material
{
public:
	DiffuseLight(Texture *tex);
	virtual bool scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const;
	virtual vec3 emitted(float u, float v, const vec3& p) const;
private:
	Texture* emit;
};