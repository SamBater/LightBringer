#include "Core/Texture/NoiseMap.h"
using namespace YYLB;
NoiseMap::NoiseMap()
{
	int size = 1024;
	h = size;
	w = size;
	comp = 3;
	sprite = new unsigned char[size * size * comp];
	for (int y = 0; y < size; y++)
		for (int x = 0; x < size; x++)
		{
			int i = w * y * 3 + x * 3;
			unsigned char color = rand() % 2 * 255;
			sprite[i] = color;
			sprite[i + 1] = color;
			sprite[i + 2] = color;
		}
}