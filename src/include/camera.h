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
	vec3 origin;			// カメラ位置（rayの原点）	rayはカメラ位置からカメラの向かっている方向に飛ばすので、カメラ位置を使ってrayを計算する
	vec3 lowerLeftCorner;	// カメラ（ray）の目標地点となる平面の左下座標
	vec3 horizontal;		// カメラ（ray）の目標地点となる平面の水平方向長さ
	vec3 vertical;			// カメラ（ray）の目標地点となる平面の垂直方向長さ
	vec3 u, v, w;			// Camera's corrdinate
	float lensRadius;		// virtual lens radius in front of film plane
	float timeOpen, timeClose; // shutter open/close time.
};