#include "yz_rect.h"
#include "aabb.h"

YzRect::YzRect()
{
}

YzRect::YzRect(float _y0, float _y1, float _z0, float _z1, float _k, material * _mat):
	y0{ _y0 },
	y1{ _y1 },
	z0{ _z0 },
	z1{ _z1 },
	mat{ _mat }
{
}

bool YzRect::hit(const ray & r, float tMin, float tMax, HitRecord & rec) const
{
	// calculate t when the ray hits the plane on the reclangle
	float t{ k - r.origin().z() / r.diretion().z() };
	if (t< tMin || t>tMax)
		return false;
	// evaluate the hit point is in the rectangle
	float y{ r.origin().y() + t*r.diretion().y() };
	float z{ r.origin().z() + t*r.diretion().z() };
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;	// not in the rectangle

	// calculate HitRecord parameters
	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.material = mat;
	rec.hitPoint = r.point_at_parameter(t);
	rec.normal = vec3{ 0.0f,0.0f,1.0f };
	return true;
}

bool YzRect::boundingBox(float t0, float t1, AABB & box) const
{
	box = AABB{ vec3{y0, z0, k - 0.0001f}, vec3{y1, t1, k + 0.0001f} };
	return true;
}
