#pragma once

class ray;
struct HitRecord;
class vec3;

class material
{
public:
	virtual bool scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const = 0;
};