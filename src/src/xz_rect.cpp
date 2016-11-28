#include "xz_rect.h"
#include "aabb.h"

XzRect::XzRect()
{
}

XzRect::XzRect(float _x0, float _x1, float _z0, float _z1, float _k, material * _mat):
	x0{ _x0 },
	x1{ _x1 },
	z0{ _z0 },
	z1{ _z1 },
	mat{ _mat }
{
}

bool XzRect::hit(const ray & r, float tMin, float tMax, HitRecord & rec) const
{
	// calculate t when the ray hits the plane on the reclangle
	float t{ k - r.origin().z() / r.diretion().z() };
	if (t< tMin || t>tMax)
		return false;
	// evaluate the hit point is in the rectangle
	float x{ r.origin().x() + t*r.diretion().x() };
	float z{ r.origin().z() + t*r.diretion().z() };
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;	// not in the rectangle

	// calculate HitRecord parameters
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.material = mat;
	rec.hitPoint = r.point_at_parameter(t);
	rec.normal = vec3{ 0.0f,0.0f,1.0f };
	return true;
}

bool XzRect::boundingBox(float t0, float t1, AABB & box) const
{
	box = AABB{ vec3{x0, z0, k - 0.0001f}, vec3{x1, t1, k + 0.0001f} };
	return true;
}
