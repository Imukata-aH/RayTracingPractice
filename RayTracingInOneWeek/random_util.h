#pragma once

#include <random>
class vec3;

class RandomUtil
{
public:
	static float getRandom0to1();
	static vec3 getrRandomPointInUnitSphere();
private:
	//static std::random_device rnd;
	static std::mt19937 mt;
	static std::uniform_real_distribution<float> dist;
};