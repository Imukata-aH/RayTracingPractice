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
		// 接触点のnormalとrayの入射角が90°未満 -> rayが物体内部から空気中へ向かっている
		outward_normal = -rec.normal;	// 法線ベクトルはrayの入射角にかかわらず、球の外側方向に向かっている。物体内部からの屈折を計算するため、法線を物体内部へ向ける
		isRefracted = refract(r_in.diretion(), outward_normal, refIdx, 1.0f, refracted); // 空気の屈折率を1とする。
	}
	else
	{
		// rayが空気中から物体内部へ向かっている
		outward_normal = rec.normal;
		isRefracted = refract(r_in.diretion(), outward_normal, 1.0f, refIdx,  refracted); // 空気の屈折率を1とする。
	}

	if (isRefracted)
	{
		scattered = ray(rec.hitPoint, refracted);
	}
	else
	{
		scattered = ray(rec.hitPoint, reflected);	// 全反射
		return false;
	}
	return true;
}