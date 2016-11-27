#include "perlin.h"
#include "vec3.h"
#include "random_util.h"

using namespace std;

namespace
{
	const size_t parameterSize{ 256 };

	// trilinear interpolation
	// c[2][2][2]: the values at each vertex of the box
	// u, v, w: a position within the box
	inline float trilinearInterp(float c[2][2][2], float u, float v, float w)
	{
		float accum{ 0 };
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				for (size_t k = 0; k < 2; k++)
				{
					accum += (i*u + (1 - i)*(1 - u))*
							 (j*v + (1 - j)*(1 - v))*
							 (k*w + (1 - k)*(1 - w))*c[i][j][k];
				}
			}
		}

		return accum;
	}

	inline float perlinInterpolation(vec3 c[2][2][2], float u, float v, float w)
	{
		// using hermite cubic
		float uu{ u*u*(3 - 2 * u) };
		float vv{ v*v*(3 - 2 * v) };
		float ww{ w*w*(3 - 2 * w) };

		float accum{ 0 };
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				for (size_t k = 0; k < 2; k++)
				{
					vec3 weight_v{ u - i, v - j,w - k };
					accum += (i*uu + (1 - i)*(1 - uu))*
							 (j*vv + (1 - j)*(1 - vv))*
							 (k*ww + (1 - k)*(1 - ww))*dot(c[i][j][k], weight_v);
				}
			}
		}

		return accum;
	}
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
	int i{ static_cast<int>(floor(p.x())) };
	int j{ static_cast<int>(floor(p.y())) };
	int k{ static_cast<int>(floor(p.z())) };
	vec3 c[2][2][2];
	for (size_t di = 0; di < 2; di++)
	{
		for (size_t dj = 0; dj < 2; dj++)
		{
			for (size_t dk = 0; dk < 2; dk++)
			{
				c[di][dj][dk] = ranVec()[permX()[(i + di)&size] ^ permY()[(j + dj) & size] ^ permZ()[(k + dk) & size]];
			}
		}
	}

	return perlinInterpolation(c, u, v, w);
}

float Perlin::turbulance(const vec3& p, int depth) const
{
	float accum{ 0.0f };
	vec3 tempP{ p };
	float weight{ 1.0f };
	for (int i = 0; i < depth; i++)
	{
		accum += weight * noise(tempP);
		weight *= 0.5f;
		tempP *= 2;			
	}
	return fabs(accum);
}

const vector<vec3>& Perlin::ranVec()
{
	static vector<vec3>* rf = perlinGenerate();
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

vector<vec3>* Perlin::perlinGenerate()
{
	vector<vec3>* p = new vector<vec3>(parameterSize);
	for (size_t i = 0; i < parameterSize; i++)
	{
		(*p)[i] = unit_vector(vec3{ -1 + 2 * RandomUtil::getRandom0to1(), -1 + 2 * RandomUtil::getRandom0to1(), -1 + 2 * RandomUtil::getRandom0to1() });
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