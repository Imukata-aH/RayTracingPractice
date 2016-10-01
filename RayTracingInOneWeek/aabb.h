#pragma once

#include "vec3.h"
#include "ray.h"

class ray;

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

// axis-aligned bounding box
class AABB
{
public:
	AABB() {};
	AABB(const vec3& minBound, const vec3& maxBound) : minBound{ minBound }, maxBound{ maxBound } {};
	vec3 min() const;
	vec3 max() const;

	// if the ray hits the AABB
	// ray[in]: ray
	// tMin[in]: used as minimum value of t when the ray glaze the bouding box (parallel with an axis). 
	// tMax[in]: used as maximum value of t when the ray glaze the bouding box (parallel with an axis). 
	bool hit(const ray& r, float tMin, float tMax)const;

private:
	vec3 minBound;
	vec3 maxBound;
};

inline vec3 AABB::min() const
{
	return minBound;
}

inline vec3 AABB::max() const
{
	return maxBound;
}

inline bool AABB::hit(const ray& r, float tMin, float tMax)const
{
	for (int i = 0; i < 3; i++)
	{
		// If r.direction() along an axis is 0, t is +Inf or -Inf. It is OK.
		// If the ray is not between the bound, t0 and t1 will 'both' be +Inf or -Inf.
		float invD{ 1.0f / r.diretion()[i] };
		float t0 {(minBound[i] - r.origin()[i]) * invD };		// the value of t for each axis when the ray hits min bound
		float t1{ (maxBound[i] - r.origin()[i]) * invD };		// the value of t for each axis when the ray hits max bound

		// A ray can go to negative direction.
		// So min and max t is given by comparing t0 and t1.
		if (invD < 0.0f)
		{
			std::swap(t0, t1);
		}

		// Comparing with t along previous axis.
		// If calculated t is NaN, comparison will be false. In the case, the second argument is adopted.
		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 < tMax ? t1 : tMax;

		// If tMin and tMax is reversed, intervals for each axis are not overlapped.
		if (tMax <= tMin)
			return false;
	}
	return true;
}