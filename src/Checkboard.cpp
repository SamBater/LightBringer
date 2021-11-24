#include "Core\Texture\Checkboard.h"
#include <stdlib.h>
using namespace YYLB;

Checkboard::Checkboard()
{
	int size = 256 ;
	h = size;
	w = size;
	comp = 3;
	sprite = new unsigned char[size * size * comp];
	for(int y = 0 ; y < size ; y++)
		for (int x = 0; x < size; x++)
		{
			int i = w * y * 3 + x * 3;
			unsigned char color = (std::sin(x/5) * std::sin(y/5)) > 0 ? 0 : 255;
			sprite[i]	  = color;
			sprite[i + 1] = color;
			sprite[i + 2] = color;
		}
}
