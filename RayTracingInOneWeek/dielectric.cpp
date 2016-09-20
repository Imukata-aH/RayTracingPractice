#include "dielectric.h"
#include "ray.h"
#include "hitable.h"
#include "vec3.h"
#include "random_util.h"

namespace
{
	// calculating reflection coefficient using shlick's approximation
	float schlick(float cosine, float ref_idx)
	{
		float r0{ (1 - ref_idx) / (1 + ref_idx) };
		r0 = r0*r0;
		return r0 + (1 - r0)*pow((1 - cosine), 5);
	}
}

bool Dielectric::scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const
{
	vec3 outward_normal{};
	vec3 reflected = reflect(r_in.diretion(), rec.normal);
	attenuation = vec3{ 1.0f, 1.0f, 1.0f };
	vec3 refracted{};
	float ni_over_nt{};
	bool isRefracted{};
	float reflectProbability{};
	float cosine{};	// ���ˌ����ƐڐG�_�̕\�ʖ@���x�N�g���̊Ԃ�cosine
	if (dot(r_in.diretion(), rec.normal) > 0)
	{
		// �ڐG�_��normal��ray�̓��ˊp��90������ -> ray�����̓��������C���֌������Ă���
		outward_normal = -rec.normal;	// �@���x�N�g����ray�̓��ˊp�ɂ�����炸�A���̊O�������Ɍ������Ă���B���̓�������̋��܂��v�Z���邽�߁A�@���𕨑̓����֌�����
		isRefracted = refract(r_in.diretion(), outward_normal, refIdx, 1.0f, refracted); // ��C�̋��ܗ���1�Ƃ���B
		float a = dot(r_in.diretion(), rec.normal) * refIdx;
		float b = r_in.diretion().length();
		cosine = a / b;						// ray�̕����x�N�g���͐��K������Ă��Ȃ��̂ŁA���̑傫���Ŋ���
	}
	else
	{
		// ray����C�����畨�̓����֌������Ă���
		outward_normal = rec.normal;
		isRefracted = refract(r_in.diretion(), outward_normal, 1.0f, refIdx,  refracted); // ��C�̋��ܗ���1�Ƃ���B
		float a = dot(r_in.diretion(), rec.normal) * refIdx;
		float b = r_in.diretion().length();
		cosine = - a / b;						// ray�̕����x�N�g���͐��K������Ă��Ȃ��̂ŁA���̑傫���Ŋ���
	}

	if (isRefracted)
	{
		reflectProbability = schlick(cosine, refIdx);
	}
	else
	{
		reflectProbability = 1.0f;
	}

	// �ꕔ�̌��������˂��邱�Ƃ��Č����邽�߂ɁAreflection coefficint �𔽎ˉ\���Ƃ��āA���˂܂��͋��܌����x�N�g�����v�Z����
	if (RandomUtil::getRandom0to1() < reflectProbability)
	{
		// ���˂���
		scattered = ray{ rec.hitPoint, reflected, r_in.time() };
	}
	else
	{
		// ���܂���
		scattered = ray{ rec.hitPoint, refracted, r_in.time() };
	}
	return true;
}