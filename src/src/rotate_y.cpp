#include "rotate_y.h"

RotateY::RotateY(Hitable * originalObject, float angle):
	original{originalObject}
{
	float radians{ (float)((M_PI / 180.0f) * angle) };
	sinTheta = sin(radians);
	cosTheta = cos(radians);
	hasBox = original->boundingBox(0, 1, bbox);
	vec3 min{ FLT_MAX, FLT_MAX, FLT_MAX };
	vec3 max{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				// rotate each vertex of the bounding box.
				float x{ i*bbox.max().x() + (1 - i)*bbox.min().x() };
				float y{ j*bbox.max().y() + (1 - j)*bbox.min().y() };
				float z{ k*bbox.max().z() + (1 - k)*bbox.min().z() };
				float newX{ cosTheta * x + sinTheta*z };
				float newZ{ -sinTheta*x + cosTheta*z };

				// record max and min points of the rotated bounding box.
				vec3 tester{ newX, y, newZ };
				for (int c = 0; c < 3; c++)
				{
					if (tester[c] > max[c])
					{
						max[c] = tester[c];
					}
					if (tester[c] < min[c])
					{
						min[c] = tester[c];
					}
				}
			}
		}
	}

	// create a rotateid bounding box from the recorded min and max points.
	bbox = AABB{ min, max };
}

bool RotateY::hit(const ray & r, float tMin, float tMax, HitRecord & rec) const
{
	// Calculate a hit record by a rotated ray.
	// And restore the hit points and normal in the record to their original coordinate.

	vec3 origin{ r.origin() };
	vec3 direction{ r.diretion() };
	origin[0] = cosTheta*r.origin()[0] - sinTheta*r.origin()[2];
	origin[2] = sinTheta*r.origin()[0] + cosTheta*r.origin()[2];
	direction[0] = cosTheta*r.diretion()[0] - sinTheta*r.diretion()[2];
	direction[2] = sinTheta*r.diretion()[0] + cosTheta*r.diretion()[2];
	ray rotatedRay{ origin, direction, r.time() };
	if(original->hit(rotatedRay, tMin, tMax, rec))
	{
		vec3 p{ rec.hitPoint };
		vec3 normal{ rec.normal };
		
		// inverse rotation. change the formula's theta to -theta
		p[0] = cosTheta*rec.hitPoint[0] + sinTheta*rec.hitPoint[2];
		p[2] = -sinTheta*rec.hitPoint[0] + cosTheta*rec.hitPoint[2];
		normal[0] = cosTheta*rec.normal[0] + sinTheta*rec.normal[2];
		normal[2] = -sinTheta*rec.normal[0] + cosTheta*rec.normal[2];
		rec.hitPoint = p;
		rec.normal = normal;
		return true;
	}
	else
	{
		return false;
	}
}

bool RotateY::boundingBox(float t0, float t1, AABB & box) const
{
	box = bbox;
	return hasBox;
}
