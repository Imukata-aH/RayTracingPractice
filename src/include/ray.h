#pragma once
#include "vec3.h"

class ray
{
public:
	ray(){}
	ray(const vec3& origin, const vec3& direction, float time = 0.0f) : A{ origin }, B{ direction }, mTime{ time } {}
	vec3 origin()const { return A; }
	vec3 diretion()const { return B; }
	float time() const { return mTime; }
	vec3 point_at_parameter(float t)const { return A + t*B; }
private:
	vec3 A;
	vec3 B;
	float mTime;	// time when a ray exists
};