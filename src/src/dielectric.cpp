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
	float cosine{};	// 入射光線と接触点の表面法線ベクトルの間のcosine
	if (dot(r_in.diretion(), rec.normal) > 0)
	{
		// 接触点のnormalとrayの入射角が90°未満 -> rayが物体内部から空気中へ向かっている
		outward_normal = -rec.normal;	// 法線ベクトルはrayの入射角にかかわらず、球の外側方向に向かっている。物体内部からの屈折を計算するため、法線を物体内部へ向ける
		isRefracted = refract(r_in.diretion(), outward_normal, refIdx, 1.0f, refracted); // 空気の屈折率を1とする。
		float a = dot(r_in.diretion(), rec.normal) * refIdx;
		float b = r_in.diretion().length();
		cosine = a / b;						// rayの方向ベクトルは正規化されていないので、その大きさで割る
	}
	else
	{
		// rayが空気中から物体内部へ向かっている
		outward_normal = rec.normal;
		isRefracted = refract(r_in.diretion(), outward_normal, 1.0f, refIdx,  refracted); // 空気の屈折率を1とする。
		float a = dot(r_in.diretion(), rec.normal) * refIdx;
		float b = r_in.diretion().length();
		cosine = - a / b;						// rayの方向ベクトルは正規化されていないので、その大きさで割る
	}

	if (isRefracted)
	{
		reflectProbability = schlick(cosine, refIdx);
	}
	else
	{
		reflectProbability = 1.0f;
	}

	// 一部の光線が反射することを再現するために、reflection coefficint を反射可能性として、反射または屈折光線ベクトルを計算する
	if (RandomUtil::getRandom0to1() < reflectProbability)
	{
		// 反射した
		scattered = ray{ rec.hitPoint, reflected, r_in.time() };
	}
	else
	{
		// 屈折した
		scattered = ray{ rec.hitPoint, refracted, r_in.time() };
	}
	return true;
}