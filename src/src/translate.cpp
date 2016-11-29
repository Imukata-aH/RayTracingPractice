#include "translate.h"
#include "ray.h"
#include "aabb.h"

Translate::Translate(Hitable * orignalObject, const vec3 & displacement):
	original{orignalObject},
	offset{displacement}
{
}

bool Translate::hit(const ray & r, float tMin, float tMax, HitRecord & rec) const
{
	// To translate object, move ray's origin (view point) and restore the hit point's coordinate to the original one.
	ray movedR{ r.origin() - offset, r.diretion(), r.time() };
	if (original->hit(movedR, tMin, tMax, rec))
	{
		rec.hitPoint += offset;
		return true;
	}
	else
	{
		return false;
	}
}

bool Translate::boundingBox(float t0, float t1, AABB & box) const
{
	if (original->boundingBox(t0, t1, box))
	{
		box = AABB{ box.min() + offset, box.max() + offset };
		return true;
	}
	else
	{
		return false;
	}
}
