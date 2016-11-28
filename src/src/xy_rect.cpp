#include "xy_rect.h"
#include "aabb.h"

XyRect::XyRect()
{
}

XyRect::XyRect(float _x0, float _x1, float _y0, float _y1, float _k, material * _mat):
	x0{ _x0 },
	x1{ _x1 },
	y0{ _y0 },
	y1{ _y1 },
	k{ _k },
	mat{ _mat }
{
}

bool XyRect::hit(const ray & r, float tMin, float tMax, HitRecord & rec) const
{
	// calculate t when the ray hits the plane on the reclangle
	float t{ (k - r.origin().z()) / r.diretion().z() };
	if (t< tMin || t>tMax)
		return false;
	// evaluate the hit point is in the rectangle
	float x{ r.origin().x() + t*r.diretion().x() };
	float y{ r.origin().y() + t*r.diretion().y() };
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;	// not in the rectangle

	// calculate HitRecord parameters
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	rec.material = mat;
	rec.hitPoint = r.point_at_parameter(t);
	rec.normal = vec3{ 0.0f,0.0f,1.0f };
	return true;
}

bool XyRect::boundingBox(float t0, float t1, AABB & box) const
{
	box = AABB{ vec3{x0, y0, k - 0.0001f}, vec3{x1, t1, k + 0.0001f} };
	return true;
}
