#pragma once

#include "texture.h"
#include "vec3.h"

class CheckerTexture : public Texture
{
public:
	CheckerTexture() {};
	CheckerTexture(Texture* t0, Texture* t1) : even{ t0 }, odd{ t1 } {};
	virtual vec3 value(float u, float v, const vec3& p) const
	{
		float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
		{
			return odd->value(u, v, p);
		}
		else
		{
			return even->value(u, v, p);
		}
	}

private:
	Texture* odd;
	Texture* even;
};