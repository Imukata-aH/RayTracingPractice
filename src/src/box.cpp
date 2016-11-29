#include "hitable_list.h"
#include "box.h"
#include "hitable.h"
#include "vec3.h"
#include "xy_rect.h"
#include "yz_rect.h"
#include "xz_rect.h"
#include "flip_normals.h"
#include "aabb.h"

Box::Box()
{
}

Box::Box(const vec3 & p0, const vec3 & p1, material * mat)
{
	// make box by 6 rectanbles
	pMin = p0;
	pMax = p1;
	std::vector<Hitable*>* list{ new std::vector<Hitable*>(6) };
	(*list)[0] = new XyRect{ p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat };
	(*list)[1] = new FlipNormals{ new XyRect{p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat} };
	(*list)[2] = new XzRect{ p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat };
	(*list)[3] = new FlipNormals{new XzRect{ p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat }};
	(*list)[4] = new YzRect{ p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat };
	(*list)[5] = new FlipNormals{ new YzRect{ p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat } };

	hitableListPtr =  new HitableList{ list };
}

bool Box::hit(const ray & r, float tMin, float tMax, HitRecord & rec) const
{
	return hitableListPtr->hit(r, tMin, tMax, rec);
}

bool Box::boundingBox(float t0, float t1, AABB& boundingbox) const
{
	boundingbox = AABB{ pMin, pMax };
	return true;
}
