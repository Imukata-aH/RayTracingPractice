#include "diffuse_light.h"
#include "texture.h"

DiffuseLight::DiffuseLight(Texture * tex) :
	emit{tex}
{
}

// Emittable material doesn't reflect lights.
bool DiffuseLight::scatter(const ray & r_in, const HitRecord & rec, vec3 & attenuation, ray & scattered) const
{
	return false;
}

vec3 DiffuseLight::emitted(float u, float v, const vec3 & p) const
{
	return emit->value(u, v, p);
}
