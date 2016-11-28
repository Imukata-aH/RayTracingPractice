#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>

#include "sphere.h"
#include "float.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "random_util.h"
#include "moving_sphere.h"
#include "constant_texture.h"
#include "checker_texture.h"
#include "bvh_node.h"
#include "noise_texture.h"
#include "image_texture.h"
#include "xy_rect.h"
#include "xz_rect.h"
#include "yz_rect.h"
#include "diffuse_light.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace
{
	std::random_device m_rnd;
	std::mt19937 mt{ m_rnd() };
	std::uniform_real_distribution<float> dist{ 0, 1 };

	const float gamma{ 2.2f };
}

Hitable* makeRandomObject(float chooseMat, float chooseObj, vec3 center, float radius)
{
	material* mat{ nullptr };
	if (chooseMat < 0.8f)
	{
		mat = new Lambertian{ new ConstantTexture(vec3{
			RandomUtil::getRandom0to1() * RandomUtil::getRandom0to1(),
			RandomUtil::getRandom0to1() * RandomUtil::getRandom0to1(),
			RandomUtil::getRandom0to1() * RandomUtil::getRandom0to1() }
		) };
	}
	else if (chooseMat < 0.9f)
	{
		mat = new Metal{
			vec3{ 0.5f * (1.0f + RandomUtil::getRandom0to1()), 0.5f * (1.0f + RandomUtil::getRandom0to1()), 0.5f * (1.0f + RandomUtil::getRandom0to1()) },
			0.5f * RandomUtil::getRandom0to1()
		};
	}
	else
	{
		mat = new Dielectric{ 1.5f };
	}

	if (chooseObj < 0.7f)
	{
		return new Sphere(center, radius, mat);
	}
	else
	{
		return new MovingSphere(center, center + vec3{ 0.0f, 0.5f * RandomUtil::getRandom0to1(), 0.0f }, 0.0f, 1.0f, radius, mat);
	}
}

Hitable* makeSceneSimpleLight()
{
	Texture* noiseTexture{ new NoiseTexture(4) };
	std::vector<Hitable*>* list{ new std::vector<Hitable*>(4) };
	(*list)[0] = new Sphere{ vec3{0.0f, -1000.001f, 0.0f}, 1000.0f, new Lambertian{noiseTexture} };
	(*list)[1] = new Sphere{ vec3{0.0f, 2.0f, 0.0f}, 2.0f, new Lambertian{noiseTexture} };
	(*list)[2] = new Sphere{ vec3{0.0f, 9.0f, 0.0f}, 2.0f, new DiffuseLight{new ConstantTexture{vec3 {4.0f, 4.0f, 4.0f}}} };
	(*list)[3] = new XyRect{ -3.0f, 3.0f, 1.0f, 3.0f, -3.0f, new DiffuseLight{ new ConstantTexture{ vec3{ 4.0f, 4.0f, 4.0f } } } };
	return new HitableList{ list };
}

Camera getCameraForSimpleLightScene(int nx, int ny)
{
	vec3 lookFrom{ 0.0f, 5.0f, 25.0f };
	vec3 lookAt{ 0.0f, 2.5f, 0.0f };
	float distToFocus = (lookFrom - lookAt).length();
	float aperture = 0.1f;
	Camera camera{ lookFrom, lookAt, vec3{ 0.0f, 1.0f, 0.0f }, 20.0f, float(nx) / float(ny), aperture, distToFocus, 0.0f, 1.0f };
	return camera;
}

Hitable* makeSceneImageTextureSphere(const char* imageFileName)
{
	int w, h, nn;
	unsigned char* texData{ stbi_load(imageFileName, &w, &h, &nn, 0) };
	material* mat = new Lambertian(new ImageTexture(texData, w, h));
	Sphere* imageSphere{ new Sphere(vec3{0.0f, 0.0f, 0.0f}, 2.0f, mat) };
	return imageSphere;
}

Camera getCameraImageTextureSphere(int nx, int ny)
{
	vec3 lookFrom{ 0.0f, 0.0f, 13.0f };
	vec3 lookAt{ 0.0f, 0.0f, 0.0f };
	float distToFocus = (lookFrom - lookAt).length();
	float aperture = 0.05f;
	Camera camera{ lookFrom, lookAt, vec3{ 0.0f, 1.0f, 0.0f }, 20.0f, float(nx) / float(ny), aperture, distToFocus, 0.0f, 1.0f };
	return camera;
}

Hitable* makeTwoPerlinSphereScene()
{
	Texture* noiseTexture = new NoiseTexture();
	std::vector<Hitable*>* list{ new std::vector<Hitable*>(2) };
	(*list)[0] = new Sphere{ vec3{0.0f, -1000.0f, 0.0f}, 1000.0f, new Lambertian{noiseTexture} };
	(*list)[1] = new Sphere{ vec3{ 0.0f, 2.0f, 0.0f }, 2.0f, new Lambertian{ noiseTexture } };
	return new HitableList{ list };
}

Camera getCameraForTwoPerlinSphereScene(int nx, int ny)
{
	vec3 lookFrom{ 13.0f, 2.0f, 3.0f };
	vec3 lookAt{ 0.0f, 0.0f, 0.0f };
	float distToFocus = (lookFrom - lookAt).length();
	float aperture = 0.05f;
	Camera camera{ lookFrom, lookAt, vec3{ 0.0f, 1.0f, 0.0f }, 20.0f, float(nx) / float(ny), aperture, distToFocus, 0.0f, 1.0f };
	return camera;
}

Hitable* makeRandomSphereScene()
{
	int sphereNum = 500;
	std::vector<Hitable*>* list{ new std::vector<Hitable*>() };
	(*list).push_back(new Sphere{
		vec3{0.0f, -2000.0f, 0.0f},
		2000.0f,
		new Lambertian{
			new CheckerTexture{new ConstantTexture{vec3{0.1f, 0.3f, 0.5f}}, new ConstantTexture{vec3{0.9f, 0.9f, 0.9f}}}
		}
	});	// ground
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat{ RandomUtil::getRandom0to1() };
			float chooseObj{ RandomUtil::getRandom0to1() };
			vec3 center{ a + 0.9f * RandomUtil::getRandom0to1(), 0.2f, b + 0.9f*RandomUtil::getRandom0to1() };
			if ((center - vec3{ 4.0f, 0.2f, 0.0f }).length() > 0.9f)
			{
				auto obj = makeRandomObject(chooseMat, chooseObj, center, 0.2f);
				(*list).push_back(makeRandomObject(chooseMat, chooseObj, center, 0.2f));
			}
		}
	}

	(*list).push_back(new Sphere{
		vec3{0.0f, 1.0f, 0.0f},
		1.0f,
		new Dielectric{1.5f}
	});
	(*list).push_back(new Sphere{
		vec3{-4.0f, 1.0f, 0.0f},
		1.0f,
		new Lambertian{new ConstantTexture(vec3{0.1f, 0.4f, 0.7f})}
	});
	(*list).push_back(new Sphere{
		vec3{4.0f, 1.0f, 0.0f},
		1.0f,
		new Metal{vec3{0.7f, 0.6f, 0.5f}, 0.0f}
	});

	return new BVHNode{ &list->front(), static_cast<int>(list->size()), 0.0f, 1.0f };
}

Camera getCameraForRandomSphereScene(int nx, int ny)
{
	vec3 lookFrom{ 15.0f, 2.5f, 6.0f };
	vec3 lookAt{ 0.0f, 0.5f, 0.0f };
	float distToFocus = (lookFrom - lookAt).length();
	float aperture = 0.1f;
	Camera camera{ lookFrom, lookAt, vec3{ 0.0f, 1.0f, 0.0f }, 20.0f, float(nx) / float(ny), aperture, distToFocus, 0.0f, 1.0f };
	return camera;
}

vec3 colorizeFromRay(const ray& r, const Hitable& world, int depth)
{
	HitRecord rec{};
	if (world.hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.material->emitted(rec.u, rec.v, rec.hitPoint);
		if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered))	// 反射回数は50回までに制限
		{
			// 再帰的に反射を計算
			return emitted + attenuation * colorizeFromRay(scattered, world, depth + 1);
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return vec3{ 0.0f, 0.0f, 0.0f };
	}
}

int main(int argc, char** argv)
{
	auto start = std::chrono::system_clock::now();

	std::string fileName = argv[1];

	std::ofstream outputFile;
	outputFile.open(fileName, std::ios::out);	// TODO: HDRにも対応できるようJXRで保存したい

	int nx{ 800 };
	int ny{ 600 };
	int ns{ 100 };
	outputFile << "P3\n" << nx << " " << ny << "\n255\n";

	Camera camera;
	Hitable* world;

	/*camera = getCameraForRandomSphereScene(nx, ny);
	world = makeRandomSphereScene();*/

	/*camera = getCameraForTwoPerlinSphereScene(nx, ny);
	world = makeTwoPerlinSphereScene();*/

	/*camera = getCameraImageTextureSphere(nx, ny);
	world = makeSceneImageTextureSphere("./texture_images/earthmap.jpg");*/

	camera = getCameraForSimpleLightScene(nx, ny);
	world = makeSceneSimpleLight();

	const float inv_gamma{ 1 / gamma };
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 color{ 0.0f, 0.0f, 0.0f };
			// アンチエイリアシングを行うため、1つのピクセルから方向を僅かに変えたrayを放ち、それらの色を平均してそのピクセルの色とする
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + dist(mt)) / float(nx);
				float v = float(j + dist(mt)) / float(ny);
				ray r{ camera.getRay(u, v) };
				color += colorizeFromRay(r, *world, 0);
			}
			color /= float(ns);

			// ガンマ補正
			color[0] = pow(color[0], inv_gamma);
			color[1] = pow(color[1], inv_gamma);
			color[2] = pow(color[2], inv_gamma);

			int ir = int(255.99*color[0]);
			int ig = int(255.99*color[1]);
			int ib = int(255.99*color[2]);
			outputFile << ir << " " << ig << " " << ib << "\n";
		}
	}

	auto end = std::chrono::system_clock::now();
	auto duration = end - start;
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
	std::cout << sec << " sec." << std::endl;
	getchar();
	return 0;
}