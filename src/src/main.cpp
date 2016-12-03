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
#include "flip_normals.h"
#include "box.h"
#include "translate.h"
#include "rotate_y.h"
#include "constant_medium.h"

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

Hitable* makeScenePutAllFeaturesTogether()
{
	std::vector<Hitable*>* worldHitableList{ new std::vector<Hitable*>() };

	// Make rectangle light.
	{
		material* lightMaterial{ new DiffuseLight{ new ConstantTexture{vec3{7.0f, 7.0f, 7.0f}} } };
		Hitable* lightReactable{ new XzRect{ 123.0f, 423.0f, 147.0f, 412.0f, 554.0f, lightMaterial } };
		(*worldHitableList).push_back(lightReactable);
	}

	// make fog (large volume sphere)
	{
		vec3 pos{ 0.0f, 0.0f, 0.0f };
		float radius{ 5000.0f };
		float reflection{ 1.5f };
		material* dielectric{ new Dielectric{reflection} };
		Hitable* boundary{ new Sphere{pos, radius, dielectric} };

		vec3 colorWhite{ 1.0f, 1.0f, 1.0f };
		Texture* constantTex{ new ConstantTexture{colorWhite} };
		float density{ 0.0001f };
		Hitable* fogVolume{ new ConstantMedium{boundary, density, constantTex} };
		(*worldHitableList).push_back(fogVolume);
	}

	// make boxes on the floor.
	{
		std::vector<Hitable*>* boxList{ new std::vector<Hitable*>() };
		material* groundMaterial{ new Lambertian{ new ConstantTexture{vec3{0.48f, 0.83f, 0.53f}} } };
		int boxNum{ 20 };
		for (int i = 0; i < boxNum; i++)
		{
			for (int j = 0; j < boxNum; j++)
			{
				float boxWidth{ 100.0f };
				float x0{ -1000.0f + i*boxWidth };
				float z0{ -1000.0f + j*boxWidth };
				float y0{ 0.0f };
				float x1{ x0 + boxWidth };
				float y1{ 100.0f*(RandomUtil::getRandom0to1() + 0.01f) };
				float z1{ z0 + boxWidth };
				Hitable* box{ new Box{vec3{x0, y0, z0}, vec3{x1, y1, z1}, groundMaterial} };
				(*boxList).push_back(box);
			}
		}
		(*worldHitableList).push_back(new BVHNode{ &(boxList->front()), (int)boxList->size(), 0.0f, 1.0f });
	}

	// make moving sphere(motion blur)
	{
		vec3 center{ 400.0f, 400.0f, 200.0f };
		vec3 moveAmount{ 30.0f, 0.0f, 0.0f };
		vec3 colorOrange{ 0.7f, 0.3f, 0.1f };
		(*worldHitableList).push_back(new MovingSphere{ center, center + moveAmount, 0.0f, 1.0f, 50.0f, new Lambertian{new ConstantTexture{colorOrange}} });
	}

	// make dielectric sphere
	{
		vec3 pos{ 260.0f, 150.0f, 45.0f };
		material* dielectric{ new Dielectric{1.5f} };
		float radius{ 50.0f };
		(*worldHitableList).push_back(new Sphere{ pos, radius, dielectric });
	}

	// make metal sphere
	{
		vec3 pos{ 0.0f, 150.0f, 145.0f };
		vec3 gray{ 0.8f, 0.8f, 0.9f };
		float fuzziness{ 10.0f };
		material* metal{ new Metal{gray, fuzziness} };
		float radius{ 50.0f };
		(*worldHitableList).push_back(new Sphere{ pos, radius, metal });
	}

	// make noise texture
	{
		vec3 pos{ 220.0f, 280.0f, 300.0f };
		float radius{ 80.0f };
		Texture* perlinTexture{ new NoiseTexture{0.1f} };
		(*worldHitableList).push_back(new Sphere(pos, radius, new Lambertian(perlinTexture)));
	}

	// make spheres in box (rotate and translate)
	{
		int sphereNum{ 1000 };
		std::vector<Hitable*>* sphereList{ new std::vector<Hitable*>() };
		float radius{ 10.0f };
		vec3 center{ 165.0f, 165.0f, 165.0f };
		vec3 colorWhite{ 0.73f, 0.73f, 0.73f };
		material* white{ new Lambertian{new ConstantTexture{colorWhite}} };
		for (int i = 0; i < sphereNum; i++)
		{
			vec3 pos{ center.x() * RandomUtil::getRandom0to1(), center.y() * RandomUtil::getRandom0to1(), center.z() * RandomUtil::getRandom0to1() };
			(*sphereList).push_back(new Sphere{ pos, radius, white });
		}

		float rotationAngle{ 15.0f };
		vec3 translateAmout{ -100.0f, 270.0f, 395.0f };
		Hitable* nodes{ new BVHNode{&(sphereList->front()), (int)sphereList->size(), 0.0f, 1.0f} };
		Hitable* rotatedY{ new RotateY{nodes, rotationAngle} };
		Hitable* translated{ new Translate{rotatedY, translateAmout} };
		(*worldHitableList).push_back(translated);
	}

	// make subsurface reflection sphere (volume inside a dielectric)
	{
		float reflection{ 1.5f };
		material* dielectric{ new Dielectric{ reflection } };
		vec3 pos{ 360.0f, 180.0f, 145.0f };
		float radius{ 70.0f };
		Hitable* surface{ new Sphere{pos, radius, dielectric} };
		(*worldHitableList).push_back(surface);

		vec3 colorBlue{ 0.2f, 0.4f, 0.9f };
		Texture* constantTex{ new ConstantTexture{colorBlue} };
		float density{ 0.2f };
		Hitable* volume{ new ConstantMedium{surface, density, constantTex} };
		(*worldHitableList).push_back(volume);
	}

	return new HitableList(worldHitableList);
}

Camera getCameraPutAllFeaturesTogetherScene(int nx, int ny)
{
	vec3 lookFrom{ 478.0f, 278.0f, -600.0f };
	vec3 lookAt{ 278.0f, 278.0f, 0.0f };
	float distToFocus{ 10.0f };
	float aperture{ 0.0f };
	float vfov{ 40.0f };
	Camera camera{ lookFrom, lookAt, vec3{ 0.0f, 1.0f, 0.0f }, vfov, float(nx) / float(ny), aperture, distToFocus, 0.0f, 1.0f };
	return camera;
}

Hitable* makeSceneSmokeyCornell()
{
	std::vector<Hitable*>* list{ new std::vector<Hitable*>() };

	material* red = new Lambertian{ new ConstantTexture{ vec3{ 0.65f, 0.05f, 0.05f } } };
	material* white = new Lambertian{ new ConstantTexture{ vec3{ 0.73f, 0.73f, 0.73f } } };
	material* green = new Lambertian{ new ConstantTexture{ vec3{ 0.12f, 0.45f, 0.15f } } };
	material* light = new DiffuseLight{ new ConstantTexture{ vec3{ 7.0f, 7.0f, 7.0f } } };

	(*list).push_back(new FlipNormals{ new YzRect{ 0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green } });	// left wall
	(*list).push_back(new YzRect{ 0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red });						// right wall
	(*list).push_back(new XzRect{ 113.0f, 443.0f, 127.0f, 432.0f, 554.0f, light });				// ceiling light
	(*list).push_back(new FlipNormals{ new XzRect{ 0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white } });	// ceiling
	(*list).push_back(new XzRect{ 0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white });						// floor
	(*list).push_back(new FlipNormals{ new XyRect{ 0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white } });	// back wall

	Hitable* box1 = new Translate{ new RotateY{ new Box{ vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 160.0f, 165.0f, 165.0f }, white }, -18.0f }, vec3{ 130.0f, 0.0f, 65.0f } };
	Hitable* box2 = new Translate{ new RotateY{ new Box{ vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 160.0f, 330.0f, 165.0f }, white }, 15.0f }, vec3{ 265.0f, 0.0f, 290.0f } };
	(*list).push_back(new ConstantMedium{ box1, 0.01f , new ConstantTexture{ vec3{ 1.0f, 1.0f, 1.0f } } });			// box1
	(*list).push_back(new ConstantMedium{ box2, 0.01f , new ConstantTexture{ vec3{ 0.0f, 0.0f, 0.0f } } });		// box2
	return new HitableList{ list };
}

Hitable* makeSceneCornellBox()
{
	std::vector<Hitable*>* list{ new std::vector<Hitable*>() };

	material* red = new Lambertian{ new ConstantTexture{vec3{0.65f, 0.05f, 0.05f}} };
	material* white = new Lambertian{ new ConstantTexture{ vec3{ 0.73f, 0.73f, 0.73f } } };
	material* green = new Lambertian{ new ConstantTexture{ vec3{ 0.12f, 0.45f, 0.15f } } };
	material* light = new DiffuseLight{ new ConstantTexture{ vec3{ 15.0f, 15.0f, 15.0f } } };

	(*list).push_back(new FlipNormals{ new YzRect{ 0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green } });	// left wall
	(*list).push_back(new YzRect{ 0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red });						// right wall
	(*list).push_back(new XzRect{ 213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light });				// ceiling light
	(*list).push_back(new FlipNormals{ new XzRect{ 0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white } });	// ceiling
	(*list).push_back(new XzRect{ 0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white });						// floor
	(*list).push_back(new FlipNormals{ new XyRect{0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white } });	// back wall
	(*list).push_back(new Translate{ new RotateY{ new Box{ vec3{ 0.0f, 0.0f, 0.0f }, vec3{ 160.0f, 165.0f, 165.0f }, white }, -18.0f }, vec3{ 130.0f, 0.0f, 65.0f } });	// box1
	(*list).push_back(new Translate{ new RotateY{ new Box{ vec3{0.0f, 0.0f, 0.0f}, vec3{160.0f, 330.0f, 165.0f}, white }, 15.0f}, vec3{ 265.0f, 0.0f, 290.0f } });		// box2
	return new HitableList{ list };
}

Camera getCameraCornellBoxScene(int nx, int ny)
{
	vec3 lookFrom{ 278.0f, 278.0f, -800.0f };
	vec3 lookAt{ 278.0f, 278.0f, 0.0f };
	float distToFocus{ 10.0f };
	float aperture{ 0.0f };
	float vfov{ 40.0f };
	Camera camera{ lookFrom, lookAt, vec3{ 0.0f, 1.0f, 0.0f }, vfov, float(nx) / float(ny), aperture, distToFocus, 0.0f, 1.0f };
	return camera;
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

	int nx{ 200 };
	int ny{ 200 };
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

	/*camera = getCameraForSimpleLightScene(nx, ny);
	world = makeSceneSimpleLight();*/

	//camera = getCameraCornellBoxScene(nx, ny);
	//world = makeSceneCornellBox();
	//world = makeSceneSmokeyCornell();

	camera = getCameraPutAllFeaturesTogetherScene(nx, ny);
	world = makeScenePutAllFeaturesTogether();

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

			if (color[0] > 1.0f)
				color[0] = 1.0f;
			if (color[1] > 1.0f)
				color[1] = 1.0f;
			if (color[2] > 1.0f)
				color[2] = 1.0f;

			// ガンマ補正
			color[0] = pow(color[0], inv_gamma);
			color[1] = pow(color[1], inv_gamma);
			color[2] = pow(color[2], inv_gamma);

			int ir = int(255.5*color[0]);
			int ig = int(255.5*color[1]);
			int ib = int(255.5*color[2]);
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