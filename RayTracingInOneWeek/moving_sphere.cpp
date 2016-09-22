#include "moving_sphere.h"
#include "ray.h"

MovingSphere::MovingSphere()
{
}

MovingSphere::MovingSphere(vec3 cent0, vec3 cent1, float t0, float t1, float r, material *m) :
	Hitable{ m }, 
	center0 {cent0}, 
	center1{ cent1 }, 
	time0{ t0 }, 
	time1{ t1 }, 
	radius{ r }
{
}

bool MovingSphere::hit(const ray& r, float tMin, float tMax, HitRecord& rec) const
{
	// rayとsphereの交点を求める方程式の解の個数から、ヒットしているかどうかを判定
	vec3 currentCenter = center(r.time());
	vec3 co = r.origin() - currentCenter;
	float a = dot(r.diretion(), r.diretion());
	float b = dot(co, r.diretion());
	float c = dot(co, co) - radius * radius;
	float discriminant = b * b - a * c;

	if (discriminant < 0) // 当たってない
	{
		return false;
	}
	else				  // 当たった
	{
		float hitPointParam = (-b - sqrt(discriminant)) / a;	// 近い方の交点のパラメータ（小さい方の解）
		if (hitPointParam > tMin && hitPointParam < tMax)
		{
			rec.t = hitPointParam;
			rec.hitPoint = r.point_at_parameter(hitPointParam);
			rec.normal = (rec.hitPoint - currentCenter) / radius;
			rec.material = mat;
			return true;
		}

		hitPointParam = (-b + sqrt(discriminant)) / a;			// 遠い方の交点のパラメータ（大きい方の解）
		if (hitPointParam > tMin && hitPointParam < tMax)
		{
			rec.t = hitPointParam;
			rec.hitPoint = r.point_at_parameter(hitPointParam);
			rec.normal = (rec.hitPoint - currentCenter) / radius;
			rec.material = mat;
			return true;
		}
	}

	return false;
}

// move linearly between center0 and center1
vec3 MovingSphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}