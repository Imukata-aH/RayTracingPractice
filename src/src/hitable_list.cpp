#include "hitable_list.h"
#include "aabb.h"

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

bool HitableList::boundingBox(float t0, float t1, AABB& box) const
{
	size_t listSize{ list->size() };
	if (listSize < 1)
		return false;

	AABB tempBox;
	bool firstTrue = (*list)[0]->boundingBox(t0, t1, tempBox);
	if (!firstTrue)
	{
		return false;
	}
	else
	{
		box = tempBox;
	}

	for (size_t i = 1; i < listSize; i++)
	{
		if ((*list)[i]->boundingBox(t0, t1, tempBox))
		{
			box = AABB::makeSurrondingBox(box, tempBox);
		}
		else
		{
			return false;
		}
	}
	
	return true;
}