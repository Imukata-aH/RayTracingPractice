#pragma once

#include <vector>

class vec3;

class Perlin
{
public:
	Perlin();
	float noise(const vec3& p) const;
private:
	static const std::vector<float>& ranFloat();
	static const std::vector<int>& permX();
	static const std::vector<int>& permY();
	static const std::vector<int>& permZ();

	static std::vector<float>* perlinGenerate();
	static void permute(std::vector<int> &p);
	static std::vector<int>* perlinGeneratePerm();
	//static std::vector<float>& ranfloat;
	//static std::vector<int>& permX;
	//static std::vector<int>& permY;
	//static std::vector<int>& permZ;
};