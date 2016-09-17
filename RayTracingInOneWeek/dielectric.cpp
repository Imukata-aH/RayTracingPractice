#include "dielectric.h"
#include "ray.h"
#include "hitable.h"
#include "vec3.h"

bool Dielectric::scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const
{
	vec3 outward_normal{};
	vec3 reflected = reflect(r_in.diretion(), rec.normal);
	attenuation = vec3{ 1.0f, 1.0f, 1.0f };
	vec3 refracted{};
	float ni_over_nt{};
	bool isRefracted{};
	if (dot(r_in.diretion(), rec.normal) > 0)
	{
		// �ڐG�_��normal��ray�̓��ˊp��90������ -> ray�����̓��������C���֌������Ă���
		outward_normal = -rec.normal;	// �@���x�N�g����ray�̓��ˊp�ɂ�����炸�A���̊O�������Ɍ������Ă���B���̓�������̋��܂��v�Z���邽�߁A�@���𕨑̓����֌�����
		isRefracted = refract(r_in.diretion(), outward_normal, refIdx, 1.0f, refracted); // ��C�̋��ܗ���1�Ƃ���B
	}
	else
	{
		// ray����C�����畨�̓����֌������Ă���
		outward_normal = rec.normal;
		isRefracted = refract(r_in.diretion(), outward_normal, 1.0f, refIdx,  refracted); // ��C�̋��ܗ���1�Ƃ���B
	}

	if (isRefracted)
	{
		scattered = ray(rec.hitPoint, refracted);
	}
	else
	{
		scattered = ray(rec.hitPoint, reflected);	// �S����
		return false;
	}
	return true;
}