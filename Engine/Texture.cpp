#include "Texture.h"
#include "glm/fwd.hpp"
#include <iterator>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "YLBFileSystem.h"
#include <cmath>

using namespace ylb;

Texture::Texture(char const* file_name,int maxComp)
{
	auto path = YLBFileSystem::GetInstance().GetAssetsPath(file_name);
	auto data = stbi_load(path.c_str(), &w, &h, &comp, 0);
	if (!data) {
		std::cerr << "ERROR: Could not load texture image file '"
			<< path << "'.\n";
		w = h = 0;
	}
	sprite = new glm::vec3[w*h];
	float inv = 1.0 / maxComp;
	for(int i = 0 ; i < w*h ; i++){
		glm::vec3 v;
		for(int j = 0 ; j < comp ; j++)
		{
			v[j] = data[i * comp + j] * inv;
		}
		sprite[i] = v;
	}

}

Texture::Texture(float* data, int w, int h , int comp , float maxComp) :w(w), h(h), comp(comp) {
	// fsprite = new float[w * h * comp];
	// for (int i = 0; i < w * h * comp; i++)
	// 	fsprite[i] = data[i];
}

glm::vec3 Texture::tex2d(const float& tex_cord_x, const float& tex_cord_y)
{
	if (sprite == nullptr) return glm::vec3{ 1,1,1 };

	//nearest
	// Clamp input texture coordinates to [0,1] x [1,0]
	float u = clamp(tex_cord_x, 0.0f, 1.0f);
	float v = 1 - clamp(tex_cord_y, 0.0f, 1.0f); // Flip V to image coordinates

	auto i = static_cast<int>(u * w + 0.5f);
	auto j = static_cast<int>(v * h + 0.5f);

	// Clamp integer mapping, since actual coordinates should be less than 1.0
	if (i >= w)
		i = w - 1;
	if (j >= h)
		j = h - 1;

	auto pixel = j * w + i;
	return sprite[pixel];




	//Bilinear Interpolation
	// float x = clamp(tex_cord_x, 0.f, 0.99f);
	// float y = 1 - clamp(tex_cord_y, 0.f, 0.99f);

	// float cx = x * w;
	// float cy = y * h;

	// int x1 = std::floor(cx);
	// int x2 = std::min(x1 + 1, w - 1);
	// int y1 = std::floor(cy);
	// int y2 = std::min(y1 + 1, h - 1);
	// float tx = cx - x1;
	// float ty = cy - y1;

	// glm::vec<2, int> u1{ x1,y1 };
	// glm::vec<2, int> u2{ x2,y1 };
	// glm::vec<2, int> v1{ x1,y2 };
	// glm::vec<2, int> v2{ x2,y2 };

	// int p1 = calc_index(u1, w, comp);
	// glm::vec3 c1 =
	// 	uchar3_to_vec3(sprite[p1], sprite[p1 + 1], sprite[p1 + 2]);
	// int p2 = calc_index(u2, w, comp); glm::vec3 c2 = uchar3_to_vec3(sprite[p2], sprite[p2 + 1], sprite[p2 + 2]);
	// int p3 = calc_index(v1, w, comp); glm::vec3 c3 = uchar3_to_vec3(sprite[p3], sprite[p3 + 1], sprite[p3 + 2]);
	// int p4 = calc_index(v2, w, comp); glm::vec3 c4 = uchar3_to_vec3(sprite[p4], sprite[p4 + 1], sprite[p4 + 2]);

	// glm::vec3 l1 = lerp(c1, c2, tx);
	// glm::vec3 l2 = lerp(c3, c4, tx);
	// glm::vec3 l3 = lerp(l1, l2, ty);

	// return l3;

}

Texture::~Texture()
{
	stbi_image_free(sprite);
}
