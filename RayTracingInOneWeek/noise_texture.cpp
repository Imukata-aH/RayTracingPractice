#include "noise_texture.h"
#include "vec3.h"

NoiseTexture::NoiseTexture()
{
}

NoiseTexture::NoiseTexture(float scale) : scale{ scale }
{
}

vec3 NoiseTexture::value(float u, float v, const vec3& p) const
{
	return vec3(1, 1, 1) * noise.noise(scale * p);
}