#include "noise_texture.h"
#include "vec3.h"

NoiseTexture::NoiseTexture()
{
}

vec3 NoiseTexture::value(float u, float v, const vec3& p) const
{
	return vec3(1, 1, 1) * noise.noise(p);
}