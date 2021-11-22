#include "Core/Texture/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"
#include "Math/Common.h"
namespace YYLB
{
    Texture::Texture()
    {
        w = 128;
        h = 128;
        comp = 3;
        texture = new unsigned char[w * h * comp];
        unsigned char color = 255;
        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
            {
                int index = y * w * 3 + x;
                texture[index] = texture[index + 1] = texture[index + 2] = color;
            }
    }

    Texture::Texture(char const *file_name)
    {
        texture = stbi_load(file_name, &w, &h, &comp, 0);
    }

    void Texture::tex2d(const float &tex_cord_x, const float &tex_cord_y, RGB &color)
    {
        int x = tex_cord_x * w;
        int y = tex_cord_y * h;
        int pixel = y * 3 * w + x * 3;
        color.x() = texture[pixel];
        color.y() = texture[pixel + 1];
        color.z() = texture[pixel + 2];
    }

    Texture::~Texture()
    {
        stbi_image_free(texture);
    }
}
