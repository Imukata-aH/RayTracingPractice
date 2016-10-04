#pragma once

#include <vector>

class vec3;

class Perlin
{
public:
	Perlin();
	float noise(const vec3& p) const;
private:
	static std::vector<float>& ranfloat;
	static std::vector<int>& permX;
	static std::vector<int>& permY;
	static std::vector<int>& permZ;
};