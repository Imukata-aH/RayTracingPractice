#pragma once

#include "aabb.h"
#include "hitable.h"

class BVHNode : public Hitable
{
public:
	BVHNode();
	BVHNode(Hitable** list, int n, float time0, float time1);
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;

	// Create a bounding box.
	// t0[in]: time of beggining of a frame
	// t1[in]: time of end of a frame
	// box[out]: a created bounding box
	virtual bool boundingBox(float t0, float t1, AABB& box) const;

private:
	Hitable* left;
	Hitable* right;
	AABB box;

	static int boxComparerX(const void* a, const void* b);
	static int boxComparerY(const void* a, const void* b);
	static int boxComparerZ(const void* a, const void* b);
};