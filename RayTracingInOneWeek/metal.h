#pragma once

#include "material.h"
#include "vec3.h"

class Metal :public material
{
public:
	Metal(const vec3& a, float fuzziness) : albedo(a)
	{
		if (fuzziness < 1.0f) fuzz = fuzziness;
		else fuzz = 1.0f;
	}
	virtual bool scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const;

	void setAlbedo(const vec3& albedo);
	const vec3& getAlbedo()const;
private:
	vec3 albedo;
	float fuzz;
};