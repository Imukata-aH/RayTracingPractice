#pragma once

class vec3;

class Texture
{
public:
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};