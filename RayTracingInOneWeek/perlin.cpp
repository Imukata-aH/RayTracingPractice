#include "perlin.h"
#include "vec3.h"
#include "random_util.h"

using namespace std;

namespace
{
	const size_t parameterSize{ 256 };
}

Perlin::Perlin()
{
}

float Perlin::noise(const vec3& p) const
{
	float u{ p.x() - floor(p.x()) };
	float v{ p.y() - floor(p.y()) };
	float w{ p.z() - floor(p.z()) };

	int size{ static_cast<int>(::parameterSize - 1) };
	int i{ int(4 * p.x()) & size };
	int j{ int(4 * p.y()) & size };
	int k{ int(4 * p.z()) & size };

	return ranFloat()[permX()[i] ^ permY()[j] ^ permZ()[k]];
}

const vector<float>& Perlin::ranFloat()
{
	static vector<float>* rf = perlinGenerate();
	return *rf;
}

const vector<int>& Perlin::permX()
{
	static vector<int>* pX = perlinGeneratePerm();
	return *pX;
}

const vector<int>& Perlin::permY()
{
	static std::vector<int>* pY = perlinGeneratePerm();
	return *pY;
}

const vector<int>& Perlin::permZ()
{
	static std::vector<int>* pZ = perlinGeneratePerm();
	return *pZ;
}

vector<float>* Perlin::perlinGenerate()
{
	vector<float>* p = new vector<float>(parameterSize);
	for (size_t i = 0; i < parameterSize; i++)
	{
		(*p)[i] = RandomUtil::getRandom0to1();
	}
	return p;
}

void Perlin::permute(vector<int> &p)
{
	size_t size{ p.size() };
	for (size_t i = size - 1; i > 0; i--)
	{
		int target = int(RandomUtil::getRandom0to1() * (i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
	return;
}

vector<int>* Perlin::perlinGeneratePerm()
{
	vector<int>* p = new vector<int>(parameterSize);
	for (size_t i = 0; i < parameterSize; i++)
	{
		(*p)[i] = static_cast<int>(i);
	}
	permute(*p);
	return p;
}