#include "noise_texture.h"
#include "vec3.h"

NoiseTexture::NoiseTexture() : scale{1.0f}
{
}

NoiseTexture::NoiseTexture(float scale) : scale{ scale }
{
}

vec3 NoiseTexture::value(float u, float v, const vec3& p) const
{
	//return vec3(1, 1, 1) * noise.turbulance(scale * p);	// direct use of turbulance
	//return vec3(1, 1, 1) * 0.5f*(1 + noise.turbulance(scale * p));				
	return vec3(1, 1, 1)*0.5f*(1 + cos(scale*p.x() + 10 * noise.turbulance(p)));	// to make color proportional to sine function.
}