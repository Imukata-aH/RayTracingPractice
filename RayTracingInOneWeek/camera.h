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

	// vfov[degree]
	Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vfov, float aspect)
	{
		vec3 w{ unit_vector(lookFrom - lookAt) };	 //Camera is faced to -w
		vec3 u{ unit_vector(cross(vUp, w)) };
		vec3 v{ cross(w, u) };

		float theta{ vfov * (float)(M_PI / 180.0) };
		float halfHeight { tan(theta / 2) };
		float halfWidth{ aspect * halfHeight };
		
		origin = lookFrom;
		lowerLeftCorner = origin - halfWidth*u - halfHeight*v - w;
		horizontal = 2 * halfWidth * u;
		vertical = 2 * halfHeight * v;
	}

	const ray getRay(const float u, const float v) { return ray{ origin, lowerLeftCorner + u * horizontal + v * vertical - origin }; }

	//TODO: getter, setter

private:
	vec3 origin;			// カメラ位置（rayの原点）	rayはカメラ位置からカメラの向かっている方向に飛ばすので、カメラ位置を使ってrayを計算する
	vec3 lowerLeftCorner;	// カメラ（ray）の目標地点となる平面の左下座標
	vec3 horizontal;		// カメラ（ray）の目標地点となる平面の水平方向長さ
	vec3 vertical;			// カメラ（ray）の目標地点となる平面の垂直方向長さ
};