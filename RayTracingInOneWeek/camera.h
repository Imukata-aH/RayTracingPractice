#pragma once

#include "ray.h"
#include "random_util.h"

namespace
{
	vec3 randomInUnitDisc()
	{
		vec3 p{};
		do
		{
			p = 2.0*vec3{ RandomUtil::getRandom0to1(), RandomUtil::getRandom0to1(), 0.0f } - vec3{ 1.0f, 1.0f, 0.0f };
		} while (dot(p, p) >= 1.0f);
		return p;
	}
}

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

	// vfov[degree]
	Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vfov, float aspect, float aperture, float focusDist)
	{
		lensRadius = aperture / 2.0f;

		w = unit_vector(lookFrom - lookAt);	 //Camera is faced to -w
		u = unit_vector(cross(vUp, w));
		v = cross(w, u);

		float theta{ vfov * (float)(M_PI / 180.0) };
		float halfHeight { tan(theta / 2) };
		float halfWidth{ aspect * halfHeight };
		
		origin = lookFrom;
		// resize and relocation the picture plane by the distance of a lens
		// focusDist �{���ꂽ�傫���� picture plane �� focusDist �{�̋����Ɉʒu�����̂ŁA�o�͂����摜�̑傫���͈�v
		lowerLeftCorner = origin - halfWidth*focusDist*u - halfHeight*focusDist*v - focusDist*w;
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}

	const ray getRay(const float s, const float t) 
	{ 
		// cast rays from a lens
		vec3 rd = lensRadius * randomInUnitDisc();
		vec3 offset = u * rd.x()  + v * rd.y();
		vec3 rayOriginOnLens = origin + offset;
		return ray{ rayOriginOnLens, lowerLeftCorner + s * horizontal + t * vertical - rayOriginOnLens }; 
	}

	//TODO: getter, setter

private:
	vec3 origin;			// �J�����ʒu�iray�̌��_�j	ray�̓J�����ʒu����J�����̌������Ă�������ɔ�΂��̂ŁA�J�����ʒu���g����ray���v�Z����
	vec3 lowerLeftCorner;	// �J�����iray�j�̖ڕW�n�_�ƂȂ镽�ʂ̍������W
	vec3 horizontal;		// �J�����iray�j�̖ڕW�n�_�ƂȂ镽�ʂ̐�����������
	vec3 vertical;			// �J�����iray�j�̖ڕW�n�_�ƂȂ镽�ʂ̐�����������
	vec3 u, v, w;			// Camera's corrdinate
	float lensRadius;		// virtual lens radius in front of film plane
};