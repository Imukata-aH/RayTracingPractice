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

namespace
{
	std::random_device m_rnd;
	std::mt19937 mt{ m_rnd() };
	std::uniform_real_distribution<float> dist{ 0, 1 };

	const float gamma{ 2.2f };
}

Hitable* makeRandomSphereScene()
{
	int sphereNum = 500;
	std::vector<Hitable*>* list{ new std::vector<Hitable*>() };
	(*list).push_back(new Sphere{ vec3{0.0f, -1000.0f, 0.0f}, 1000.0f, new Lambertian{vec3{0.5f, 0.5f, 0.5f}} });	// ground
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat{ RandomUtil::getRandom0to1() };
			vec3 center{ a + 0.9f * RandomUtil::getRandom0to1(), 0.2f, b + 0.9f*RandomUtil::getRandom0to1() };
			if ((center - vec3{ 4.0f, 0.2f, 0.0f }).length() > 0.9f)
			{
				if (chooseMat < 0.8f)
				{
					// diffuse
					(*list).push_back(new Sphere{ 
						center, 
						0.2f, 
						new Lambertian{vec3{RandomUtil::getRandom0to1() * RandomUtil::getRandom0to1(), RandomUtil::getRandom0to1() * RandomUtil::getRandom0to1(), RandomUtil::getRandom0to1() * RandomUtil::getRandom0to1()}} 
					});
				}
				else if (chooseMat < 0.95f)
				{
					// metal
					(*list).push_back(new Sphere{
						center,
						0.2f,
						new Metal{
							vec3{0.5f * (1.0f + RandomUtil::getRandom0to1()), 0.5f * (1.0f + RandomUtil::getRandom0to1()), 0.5f * (1.0f + RandomUtil::getRandom0to1())},
							0.5f * RandomUtil::getRandom0to1()
						}
					});
				}
				else
				{
					// glass
					(*list).push_back(new Sphere{ center, 0.2f, new Dielectric{1.5f} });
				}
			}
		}
	}

	(*list).push_back(new Sphere{ vec3{0.0f, 1.0f, 0.0f}, 1.0f, new Dielectric{1.5f} });
	(*list).push_back(new Sphere{ vec3{-4.0f, 1.0f, 0.0f}, 1.0f, new Lambertian{vec3{0.1f, 0.4f, 0.7f}} });
	(*list).push_back(new Sphere{ vec3{4.0f, 1.0f, 0.0f}, 1.0f, new Metal{vec3{0.7f, 0.6f, 0.5f}, 0.0f} });

	return new HitableList{ list };
}

vec3 colorizeFromRay(const ray& r, const Hitable& world, int depth)
{
	HitRecord rec{};
	if (world.hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered))	// 反射回数は50回までに制限
		{
			// 再帰的に反射を計算
			return attenuation * colorizeFromRay(scattered, world, depth + 1);
		}
		else
		{
			return vec3{ 0, 0, 0 };
		}
	}
	else
	{
		// 最終的にヒットしなかったrayは周囲の色（今回は方向ベクトルのy成分を使って空の色に見立てている）を返す
		vec3 unit_direction = unit_vector(r.diretion());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
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

	vec3 lookFrom{ 15.0f, 2.5f, 6.0f };
	vec3 lookAt{ 0.0f, 0.5f, 0.0f };
	float distToFocus = (lookFrom - lookAt).length();
	float aperture = 0.1f;
	Camera camera{ lookFrom, lookAt, vec3{0.0f, 1.0f, 0.0f}, 20.0f, float(nx) / float(ny), aperture, distToFocus };

	/*std::vector<Hitable*> sphereList{};
	sphereList.push_back({ new Sphere{ vec3{ 0.0f, 0.0f, -1.0f }, 0.5f, new Lambertian(vec3{0.8f, 0.3f, 0.3f})} });
	sphereList.push_back({ new Sphere{ vec3{ 0.0f, -100.5f, -1.0f }, 100.0f, new Lambertian(vec3{ 0.8f, 0.8f, 0.0f }) } });
	sphereList.push_back({ new Sphere{ vec3{ 1.0f, 0.0f, -1.0f }, 0.5f, new Metal(vec3{ 0.8f, 0.6f, 0.2f }, 0.3f) } });
	sphereList.push_back({ new Sphere{ vec3{ -1.0f, 0.0f, -1.0f }, 0.5f, new Dielectric(1.5f) } });
	sphereList.push_back({ new Sphere{ vec3{ -1.0f, 0.0f, -1.0f }, -0.45f, new Dielectric(1.5f) } });
	Hitable* world = new HitableList{ &sphereList };*/
	Hitable* world = makeRandomSphereScene();

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