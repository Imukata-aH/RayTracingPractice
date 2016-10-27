#include "lambertian.h"
#include "hitable.h"
#include "ray.h"
#include "vec3.h"
#include "random_util.h"
#include "texture.h"

Lambertian::Lambertian(Texture* t)
	: albedo(t)
{
	
}

bool Lambertian::scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const
{
	// �ڐG�_�ɐڂ���P�ʋ����ʓ��̃����_���ȓ_���A���˕����Ƃ���
	vec3 target = rec.hitPoint + rec.normal + RandomUtil::getrRandomPointInUnitSphere();
	scattered = ray(rec.hitPoint, target - rec.hitPoint, r_in.time());
	attenuation = albedo->value(0, 0, rec.hitPoint);
	return true;
}