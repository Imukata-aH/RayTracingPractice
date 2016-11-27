#include "image_texture.h"
#include "vec3.h"

ImageTexture::ImageTexture()
{
}

ImageTexture::ImageTexture(unsigned char* pixels, int w, int h) : 
	data{ pixels }, 
	width{ w }, 
	height{ h }
{
}

vec3 ImageTexture::value(float u, float v, const vec3& p) const
{
	int i{ u*width };
	int j{ (1.0f - v)*height - 0.001f };	// invert direction of y axis
	if (i < 0)	i = 0;
	if (j < 0)j = 0;
	if (i > width - 1)i = width - 1;
	if (j > height - 1)j = height - 1;
	float r{ int(data[3 * i + 3 * width*j + 0]) / 255.0f };
	float g{ int(data[3 * i + 3 * width*j + 1]) / 255.0f };
	float b{ int(data[3 * i + 3 * width*j + 2]) / 255.0f };
	return vec3{ r,g,b };
}