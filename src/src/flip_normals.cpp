#include "flip_normals.h"

FlipNormals::FlipNormals(Hitable * object):
	obj{object}
{
}

bool FlipNormals::hit(const ray & r, float tMin, float tMax, HitRecord & rec) const
{
	if (obj->hit(r, tMin, tMax, rec))
	{
		// flip normal
		rec.normal = -rec.normal;
		return true;
	}
	else
	{
		return false;
	}
}

bool FlipNormals::boundingBox(float t0, float t1, AABB & box) const
{
	return obj->boundingBox(t0, t1, box);
}
