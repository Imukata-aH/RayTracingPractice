﻿#include <math.h>

#include "sphere.h"
#include "ray.h"
#include "aabb.h"

bool Sphere::hit(const ray& r, float tMin, float tMax, HitRecord& rec) const
{
	// rayとsphereの交点を求める方程式の解の個数から、ヒットしているかどうかを判定
	vec3 co = r.origin() - center;
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
			rec.normal = (rec.hitPoint - center) / radius;
			rec.material = mat;
			getSphereUV((rec.hitPoint - this->center) / radius, rec.u, rec.v);	// calculate uv as unit sphere
			return true;
		}

		hitPointParam = (-b + sqrt(discriminant)) / a;			// 遠い方の交点のパラメータ（大きい方の解）
		if (hitPointParam > tMin && hitPointParam < tMax)
		{
			rec.t = hitPointParam;
			rec.hitPoint = r.point_at_parameter(hitPointParam);
			rec.normal = (rec.hitPoint - center) / radius;
			rec.material = mat;
			getSphereUV((rec.hitPoint - this->center) / radius, rec.u, rec.v);	// calculate uv as unit sphere
			return true;
		}
	}

	return false;
}

bool Sphere::boundingBox(float t0, float t1, AABB& box) const
{
	box = AABB{ center - vec3{radius, radius, radius}, center + vec3{radius, radius, radius} };
	return true;
}

void Sphere::getSphereUV(const vec3& p, float& u, float& v) const
{
	// calculate spherical coordinate of the hit point
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	// normalization
	u = 1.0f - (phi + (float)M_PI) / (2.0f * (float)M_PI);
	v = (theta + (float)M_PI / 2.0f) / (float)M_PI;
}