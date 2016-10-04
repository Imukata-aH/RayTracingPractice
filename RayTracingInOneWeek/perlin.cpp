#include "perlin.h"
#include "vec3.h"
#include "random_util.h"

using namespace std;

namespace
{
	size_t parameterSize{ 256 };

	static vector<float>* perlinGenerate()
	{
		//size_t size{ parameterSize };
		vector<float>* p = new vector<float>(parameterSize);
		for (size_t i = 0; i < parameterSize; i++)
		{
			(*p)[i] = RandomUtil::getRandom0to1();
		}
		return p;
	}

	static void permute(vector<int> &p)
	{
		size_t size{ p.size() };
		for (size_t i = size - 1; i > 0 ; i--)
		{
			int target = int(RandomUtil::getRandom0to1() * (i + 1));
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
		return;
	}

	static vector<int>* perlinGeneratePerm()
	{
		//size_t size{ size };
		vector<int>* p = new vector<int>(parameterSize);
		for (size_t i = 0; i < parameterSize; i++)
		{
			(*p)[i] = static_cast<int>(i);
		}
		permute(*p);
		return p;
	}
}

vector<float>& Perlin::ranfloat = *perlinGenerate();
vector<int>& Perlin::permX = *perlinGeneratePerm();
vector<int>& Perlin::permY = *perlinGeneratePerm();
vector<int>& Perlin::permZ = *perlinGeneratePerm();

float Perlin::noise(const vec3& p) const
{
	float u{ p.x() - floor(p.x()) };
	float v{ p.y() - floor(p.y()) };
	float w{ p.z() - floor(p.z()) };

	int size{ static_cast<int>(::parameterSize - 1) };
	int i{ int(4 * p.x()) & size };
	int j{ int(4 * p.y()) & size };
	int k{ int(4 * p.z()) & size };

	return ranfloat[permX[i] ^ permY[j] ^ permZ[k]];
}