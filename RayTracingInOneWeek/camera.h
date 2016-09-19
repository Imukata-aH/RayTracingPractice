#pragma once

#include "ray.h"

class Camera
{
public:
	Camera();
	// vfov[degree]
	Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vfov, float aspect, float aperture, float focusDist, float shutterOpen, float shutterClose);

	const ray getRay(const float s, const float t) const;

private:
	vec3 origin;			// �J�����ʒu�iray�̌��_�j	ray�̓J�����ʒu����J�����̌������Ă�������ɔ�΂��̂ŁA�J�����ʒu���g����ray���v�Z����
	vec3 lowerLeftCorner;	// �J�����iray�j�̖ڕW�n�_�ƂȂ镽�ʂ̍������W
	vec3 horizontal;		// �J�����iray�j�̖ڕW�n�_�ƂȂ镽�ʂ̐�����������
	vec3 vertical;			// �J�����iray�j�̖ڕW�n�_�ƂȂ镽�ʂ̐�����������
	vec3 u, v, w;			// Camera's corrdinate
	float lensRadius;		// virtual lens radius in front of film plane
	float timeOpen, timeClose; // shutter open/close time.
};