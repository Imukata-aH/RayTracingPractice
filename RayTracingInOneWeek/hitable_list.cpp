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
	int listSize = list->size();
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

	for (int i = 1; i < listSize; i++)
	{
		if ((*list)[i]->boundingBox(t0, t1, tempBox))
		{
			box = makeSurrondingBox(box, tempBox);
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

AABB HitableList::makeSurrondingBox(AABB& box0, AABB& box1) const
{
	vec3 smallBound
	{
		fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z())
	};

	vec3 bigBound
	{
		fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z())
	};

	return AABB{ smallBound, bigBound };
}