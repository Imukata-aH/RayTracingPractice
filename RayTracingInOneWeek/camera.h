#pragma once

#include "ray.h"

class Camera
{
public:
	Camera():
		origin(vec3{ 0.0f, 0.0f, 0.0f }),
		lowerLeftCorner(vec3{ -2.0f, -1.0f, -1.0f }),
		horizontal(vec3{ 4.0f, 0.0f, 0.0f }),
		vertical(vec3{ 0.0f, 2.0f, 0.0f })
	{
	}

	Camera(vec3& origin, vec3& lowerLeftCorner, vec3& horizontal, vec3& vertical) :
		origin(origin),
		lowerLeftCorner(lowerLeftCorner),
		horizontal(horizontal),
		vertical(vertical)
	{
	}

	Camera(float vfov, float aspect)
	{
		float theta{ vfov * (float)(M_PI / 180.0) };
		float halfHeight { tan(theta / 2) };
		float halfWidth{ aspect * halfHeight };
		lowerLeftCorner = vec3{ -halfWidth, -halfHeight, -1.0f };
		horizontal = vec3{ 2 * halfWidth, 0.0f, 0.0f };
		vertical = vec3{ 0.0, 2 * halfHeight,0.0f };
		origin = vec3{ 0.0f, 0.0f, 0.0f };
	}

	const ray getRay(const float u, const float v) { return ray{ origin, lowerLeftCorner + u * horizontal + v * vertical - origin }; }

	//TODO: getter, setter

private:
	vec3 origin;			// �J�����ʒu�iray�̌��_�j	ray�̓J�����ʒu����J�����̌������Ă�������ɔ�΂��̂ŁA�J�����ʒu���g����ray���v�Z����
	vec3 lowerLeftCorner;	// �J�����iray�j�̖ڕW�n�_�ƂȂ镽�ʂ̍������W
	vec3 horizontal;		// �J�����iray�j�̖ڕW�n�_�ƂȂ镽�ʂ̐�����������
	vec3 vertical;			// �J�����iray�j�̖ڕW�n�_�ƂȂ镽�ʂ̐�����������
};