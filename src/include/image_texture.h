#pragma once

#include "texture.h"

class ImageTexture : public Texture
{
public:
	ImageTexture();
	ImageTexture(unsigned char *pixels, int w, int h);
	virtual vec3 value(float u, float v, const vec3& p) const;
private:
	unsigned char* data;
	int width, height;
};