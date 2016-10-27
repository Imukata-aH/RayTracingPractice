#include "sphere.h"
#include "ray.h"
#include "aabb.h"

bool Sphere::hit(const ray& r, float tMin, float tMax, HitRecord& rec) const
{
	// ray��sphere�̌�_�����߂�������̉��̌�����A�q�b�g���Ă��邩�ǂ����𔻒�
	vec3 co = r.origin() - center;
	float a = dot(r.diretion(), r.diretion());
	float b = dot(co, r.diretion());
	float c = dot(co, co) - radius * radius;
	float discriminant = b * b - a * c;

	if (discriminant < 0) // �������ĂȂ�
	{
		return false;
	}
	else				  // ��������
	{
		float hitPointParam = (-b - sqrt(discriminant)) / a;	// �߂����̌�_�̃p�����[�^�i���������̉��j
		if (hitPointParam > tMin && hitPointParam < tMax)
		{
			rec.t = hitPointParam;
			rec.hitPoint = r.point_at_parameter(hitPointParam);
			rec.normal = (rec.hitPoint - center) / radius;
			rec.material = mat;
			return true;
		}

		hitPointParam = (-b + sqrt(discriminant)) / a;			// �������̌�_�̃p�����[�^�i�傫�����̉��j
		if (hitPointParam > tMin && hitPointParam < tMax)
		{
			rec.t = hitPointParam;
			rec.hitPoint = r.point_at_parameter(hitPointParam);
			rec.normal = (rec.hitPoint - center) / radius;
			rec.material = mat;
			return true;
		}
	}

	return false;
}

bool Sphere::boundingBox(float t0, float t1, AABB& box) const
{
	box = AABB{ center - vec3{radius, radius, radius}, center + vec3{radius, radius, radius} };
	return true;
}