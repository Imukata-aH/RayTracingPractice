﻿#include "lambertian.h"
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
	// 接触点に接する単位球球面内のランダムな点を、反射方向とする
	vec3 target = rec.hitPoint + rec.normal + RandomUtil::getrRandomPointInUnitSphere();
	scattered = ray(rec.hitPoint, target - rec.hitPoint, r_in.time());
	attenuation = albedo->value(0, 0, rec.hitPoint);
	return true;
}