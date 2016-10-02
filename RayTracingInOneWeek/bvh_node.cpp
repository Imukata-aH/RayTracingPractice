#include "bvh_node.h"
#include "random_util.h"

BVHNode::BVHNode()
{
}

BVHNode::BVHNode(Hitable** l, int n, float time0, float time1)
{
	int axis = int(3 * RandomUtil::getRandom0to1());
	if (axis == 0)
	{
		qsort(l, n, sizeof(Hitable*), boxComparerX);
	}
	else if (axis == 0)
	{
		qsort(l, n, sizeof(Hitable*), boxComparerY);
	}
	else
	{
		qsort(l, n, sizeof(Hitable*), boxComparerZ);
	}

	if (n == 1)
	{
		// Copy an object on both nodes
		left = right = l[0];
	}
	else if (n == 2)
	{
		left = l[0];
		right = l[1];
	}
	else
	{
		// Divide a list and construct nodes recursively
		left = new BVHNode{ l, n / 2, time0, time1 };
		right = new BVHNode{ l + n / 2, n - n / 2, time0, time1 };
	}
	AABB boxLeft{}, boxRight{};
	if (!left->boundingBox(time0, time1, boxLeft) || !right->boundingBox(time0, time1, boxRight))
	{
		std::cerr << "no bounding box inBVHNode constructor\n";
	}
	box = AABB::makeSurrondingBox(boxLeft, boxRight);
}

bool BVHNode::boundingBox(float t0, float t1, AABB& b)const
{
	b = box;
	return true;
}

bool BVHNode::hit(const ray& r, float tMin, float tMax, HitRecord& rec) const
{
	if (!box.hit(r, tMin, tMax))
	{
		return false;
	}
	else
	{
		// Check left and right node.
		HitRecord leftRec{}, rightRec{};
		bool hitLeft{ left->hit(r, tMin, tMax, leftRec) };
		bool hitRight{ right->hit(r, tMin, tMax, rightRec) };
		if (hitLeft && hitRight)
		{
			if (leftRec.t < rightRec.t)
			{
				rec = leftRec;
			}
			else
			{
				rec = rightRec;
			}
			return true;
		}
		else if (hitLeft)
		{
			rec = leftRec;
			return true;
		}
		else if (hitRight)
		{
			rec = rightRec;
			return true;
		}
		else
		{
			return false;
		}
	}
}

int BVHNode::boxComparerX(const void* a, const void* b)
{
	AABB boxLeft{}, boxRight;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;

	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
	{
		std::cerr << "no bounding box in BVHNode constructor\n";
	}

	if (boxLeft.min().x() - boxRight.min().x() < 0.0f)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int BVHNode::boxComparerY(const void* a, const void* b)
{
	AABB boxLeft{}, boxRight;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;

	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
	{
		std::cerr << "no bounding box in BVHNode constructor\n";
	}

	if (boxLeft.min().y() - boxRight.min().y() < 0.0f)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int BVHNode::boxComparerZ(const void* a, const void* b)
{
	AABB boxLeft{}, boxRight;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;

	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
	{
		std::cerr << "no bounding box in BVHNode constructor\n";
	}

	if (boxLeft.min().z() - boxRight.min().z() < 0.0f)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}