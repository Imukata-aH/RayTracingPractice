#include "constant_medium.h"
#include "texture.h"
#include "aabb.h"
#include "isotropic.h"

ConstantMedium::ConstantMedium(Hitable * boundaryObject, float density, Texture * phase) :
	boundary{boundaryObject},
	density{density}
{
	phaseFunction = new Isotropic(phase);
}

bool ConstantMedium::hit(const ray & r, float tMin, float tMax, HitRecord & rec) const
{
	bool db{ RandomUtil::getRandom0to1() < 0.00001f };
	db = false;
	HitRecord rec1{}, rec2{};
	if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1))
	{
		if (boundary->hit(r, rec1.t + 0.0001f, FLT_MAX, rec2))
		{
			if (db)
				std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << "\n";

			if (rec1.t < tMin)
				rec1.t = tMin;
			if (rec2.t > tMax)
				rec2.t = tMax;
			if (rec1.t >= rec2.t)
				return false;
			if (rec1.t < 0)
				rec1.t = 0;

			float distanceInsideBoundary{ (rec2.t - rec1.t) * r.diretion().length() };
			float hitDistance{ -(1.0f / density)*log(RandomUtil::getRandom0to1()) };
			if (hitDistance < distanceInsideBoundary)
			{
				if (db)
					std::cerr << "hitDistance = " << hitDistance << "\n";
				rec.t = rec1.t + hitDistance / r.diretion().length();

				if (db)
					std::cerr << "rec.t = " << rec.t << "\n";
				rec.hitPoint = r.point_at_parameter(rec.t);

				if (db)
					std::cerr << "rec.hitPoint = " << rec.hitPoint << "\n";
				rec.normal = vec3{ 1.0f, 0.0f, 0.0f };
				rec.material = phaseFunction;
				return true;
			}
		}
	}
	return false;
}

bool ConstantMedium::boundingBox(float t0, float t1, AABB & box) const
{
	return boundary->boundingBox(t0, t1, box);
}
