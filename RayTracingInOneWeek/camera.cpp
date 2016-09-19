#include "camera.h"
#include "random_util.h"

namespace
{
	vec3 randomInUnitDisc()
	{
		vec3 p{};
		do
		{
			p = 2.0*vec3{ RandomUtil::getRandom0to1(), RandomUtil::getRandom0to1(), 0.0f } -vec3{ 1.0f, 1.0f, 0.0f };
		} while (dot(p, p) >= 1.0f);
		return p;
	}
}

Camera::Camera() :
	origin(vec3{ 0.0f, 0.0f, 0.0f }),
	lowerLeftCorner(vec3{ -2.0f, -1.0f, -1.0f }),
	horizontal(vec3{ 4.0f, 0.0f, 0.0f }),
	vertical(vec3{ 0.0f, 2.0f, 0.0f })
{
}

// vfov[degree]
Camera::Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vfov, float aspect, float aperture, float focusDist, float shutterOpen, float shutterClose) :
	timeOpen{shutterOpen},
	timeClose{shutterClose},
	lensRadius{ aperture / 2.0f}
{
	w = unit_vector(lookFrom - lookAt);	 //Camera is faced to -w
	u = unit_vector(cross(vUp, w));
	v = cross(w, u);

	float theta{ vfov * (float)(M_PI / 180.0) };
	float halfHeight{ tan(theta / 2) };
	float halfWidth{ aspect * halfHeight };

	origin = lookFrom;
	// resize and relocation the picture plane by the distance of a lens
	// focusDist ”{‚³‚ê‚½‘å‚«‚³‚Ì picture plane ‚ª focusDist ”{‚Ì‹——£‚ÉˆÊ’u‚³‚ê‚é‚Ì‚ÅAo—Í‚³‚ê‚é‰æ‘œ‚Ì‘å‚«‚³‚Íˆê’v
	lowerLeftCorner = origin - halfWidth*focusDist*u - halfHeight*focusDist*v - focusDist*w;
	horizontal = 2 * halfWidth * focusDist * u;
	vertical = 2 * halfHeight * focusDist * v;
}

const ray Camera::getRay(const float s, const float t) const
{
	// cast rays from a lens
	vec3 rd = lensRadius * randomInUnitDisc();
	vec3 offset = u * rd.x() + v * rd.y();
	vec3 rayOriginOnLens = origin + offset;
	float time = timeOpen + RandomUtil::getRandom0to1() * (timeClose - timeOpen);
	return ray{ rayOriginOnLens, lowerLeftCorner + s * horizontal + t * vertical - rayOriginOnLens, time };
}