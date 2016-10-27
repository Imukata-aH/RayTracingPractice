#pragma once

#include <vector>

class vec3;

class Perlin
{
public:
	Perlin();
	float noise(const vec3& p) const;
	float turbulance(const vec3& p, int depth = 7) const;
private:
	static const std::vector<vec3>& ranVec();
	static const std::vector<int>& permX();
	static const std::vector<int>& permY();
	static const std::vector<int>& permZ();

	static std::vector<vec3>* perlinGenerate();
	static void permute(std::vector<int> &p);
	static std::vector<int>* perlinGeneratePerm();
};