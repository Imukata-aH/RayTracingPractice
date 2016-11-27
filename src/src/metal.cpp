#include "metal.h"
#include "hitable.h"
#include "ray.h"
#include "vec3.h"
#include "random_util.h"

bool Metal::scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const
{
	vec3 reflected = material::reflect(unit_vector(r_in.diretion()), rec.normal);
	scattered = ray(rec.hitPoint, reflected + fuzz * RandomUtil::getrRandomPointInUnitSphere(), r_in.time());
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