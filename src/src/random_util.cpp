#include "random_util.h"
#include "vec3.h"

namespace
{
	static std::random_device rnd;
}

std::mt19937 RandomUtil::mt{ rnd() };
std::uniform_real_distribution<float> RandomUtil::dist{ 0, 1 };

float RandomUtil::getRandom0to1()
{
	return dist(mt);
}

// ランダムな反射方向を決定するため、rejection sampling（モンテカルロ法）を用いてランダムな点を決定する
// xyzそれぞれが[-1, 1]となる立方体からランダムに点を選び、原点からのその点へのベクトルの長さが1以上となる点を受け入れる。
vec3 RandomUtil::getrRandomPointInUnitSphere()
{
	vec3 p{};
	do
	{
		p = 2.0*vec3{ dist(mt), dist(mt), dist(mt) } -vec3{ 1.0f, 1.0f, 1.0f };
	} while (p.squared_length() >= 1.0f);
	return p;
}