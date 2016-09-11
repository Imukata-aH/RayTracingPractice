#include "metal.h"
#include "hitable.h"
#include "ray.h"
#include "vec3.h"
#include "random_util.h"

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

bool Metal::scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const
{
	vec3 reflected = reflect(unit_vector(r_in.diretion()), rec.normal);
	scattered = ray(rec.hitPoint, reflected + fuzz * RandomUtil::getrRandomPointInUnitSphere());
	attenuation = albedo;
	return (dot(scattered.diretion(), rec.normal) > 0);
}

void Metal::setAlbedo(const vec3& albedo)
{
	this->albedo = albedo;
}

const vec3& Metal::getAlbedo()const
{
	return albedo;
}