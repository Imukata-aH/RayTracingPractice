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

// �����_���Ȕ��˕��������肷�邽�߁Arejection sampling�i�����e�J�����@�j��p���ă����_���ȓ_�����肷��
// xyz���ꂼ�ꂪ[-1, 1]�ƂȂ闧���̂��烉���_���ɓ_��I�сA���_����̂��̓_�ւ̃x�N�g���̒�����1�ȏ�ƂȂ�_���󂯓����B
vec3 RandomUtil::getrRandomPointInUnitSphere()
{
	vec3 p{};
	do
	{
		p = 2.0*vec3{ dist(mt), dist(mt), dist(mt) } -vec3{ 1.0f, 1.0f, 1.0f };
	} while (p.squared_length() >= 1.0f);
	return p;
}