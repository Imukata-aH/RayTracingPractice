﻿#pragma once

#include "texture.h"
#include "perlin.h"

class NoiseTexture : public Texture
{
public:
	NoiseTexture();
	NoiseTexture(float scale);
	virtual vec3 value(float u, float v, const vec3& p) const;

private:
	Perlin noise;
	float scale;
};