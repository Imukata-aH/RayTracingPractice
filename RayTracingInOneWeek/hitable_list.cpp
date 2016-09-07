#include "hitable_list.h"

bool HitableList::hit(const ray& r, float tMin, float tMax, HitRecord& rec) const
{
	HitRecord tempRec;
	bool isHitAnything = false;
	double closestSoFar = tMax;

	size_t listSize { getListSize() };
	for (size_t i = 0; i < listSize; i++)
	{
		if ((*list)[i]->hit(r, tMin, tMax, tempRec))
		{
			isHitAnything = true;
			if (tempRec.t < closestSoFar)
			{
				closestSoFar = tempRec.t;
				rec = tempRec;
			}
		}
	}
	return isHitAnything;
}