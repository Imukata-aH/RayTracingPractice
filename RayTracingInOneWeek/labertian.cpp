#include "lambertian.h"
#include "hitable.h"
#include "ray.h"
#include "vec3.h"
#include "random_util.h"

bool Lambertian::scatter(const ray& r_in, const HitRecord& rec, vec3& attenuation, ray& scattered) const
{
	// �ڐG�_�ɐڂ���P�ʋ����ʓ��̃����_���ȓ_���A���˕����Ƃ���
	vec3 target = rec.hitPoint + rec.normal + RandomUtil::getrRandomPointInUnitSphere();
	scattered = ray(rec.hitPoint, target - rec.hitPoint);
	attenuation = albedo;
	return true;
}

void Lambertian::setAlbedo(const vec3& albedo)
{
	this->albedo = albedo;
}

const vec3& Lambertian::getAlbedo()const
{
	return albedo;
}