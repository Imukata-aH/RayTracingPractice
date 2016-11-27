#pragma once

#include "vec3.h"
#include "texture.h"

class ConstantTexture : public Texture
{
public:
	ConstantTexture() {};
	ConstantTexture(vec3 color) : color{ color } {};
	virtual vec3 value(float u, float v, const vec3& p) const
	{
		return color;
	};

private:
	vec3 color;
};