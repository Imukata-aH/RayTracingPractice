#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include "sphere.h"
#include "float.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"


namespace
{
	std::random_device m_rnd;
	std::mt19937 mt{ m_rnd() };
	std::uniform_real_distribution<float> dist{ 0, 1 };

	const float gamma{ 2.2f };
}

// ランダムな反射方向を決定するため、rejection sampling（モンテカルロ法）を用いてランダムな点を決定する
// xyzそれぞれが[-1, 1]となる立方体からランダムに点を選び、原点からのその点へのベクトルの長さが1以上となる点を受け入れる。
vec3 randomInUnitSphere()
{
	vec3 p{};
	do
	{
		p = 2.0*vec3{ dist(mt), dist(mt), dist(mt) } -vec3{ 1.0f, 1.0f, 1.0f };
	} while (p.squared_length() >= 1.0f);
	return p;
}

vec3 colorizeFromRay(const ray& r, const Hitable& world, int depth)
{
	HitRecord rec{};
	if (world.hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered))
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
	std::string fileName = argv[1];

	std::ofstream outputFile;
	outputFile.open(fileName, std::ios::out);	// TODO: HDRにも対応できるようJXRで保存したい

	int nx{ 200 };
	int ny{ 100 };
	int ns{ 100 };
	outputFile << "P3\n" << nx << " " << ny << "\n255\n";

	Camera camera{};

	std::vector<Hitable*> sphereList{2};
	sphereList[0] = { new Sphere{ vec3{ 0.0f, 0.0f, -1.0f }, 0.5f, new Lambertian(vec3{0.8f, 0.3f, 0.3f})} };
	sphereList[1] = { new Sphere{ vec3{ 0.0f, -100.5f, -1.0f }, 100.0f, new Lambertian(vec3{ 0.8f, 0.8f, 0.0f }) } };
	Hitable* world = new HitableList{ &sphereList };

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

	return 0;
}