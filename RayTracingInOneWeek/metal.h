#pragma once

#include "material.h"
#include "vec3.h"

class Metal :public material
{
public:
	Metal(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const;

	void setAlbedo(const vec3& albedo);
	const vec3& getAlbedo()const;
private:
	vec3 albedo;
};