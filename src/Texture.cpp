#include "Core/Texture/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"


namespace YYLB
{
    Texture::Texture(char const *file_name)
    {
        sprite = stbi_load(file_name, &w, &h, &comp, 0);
    }

    Vec3f uchar3_to_vec3(unsigned char r, unsigned char g, unsigned char b)
    {
        static float inv = 1 / 255.f;
        return Vec3f{ r * inv,g * inv,b * inv };
    }

    Vec3f Texture::tex2d(const float &tex_cord_x, const float &tex_cord_y)
    {
        int x = tex_cord_x * w;
        int y = tex_cord_y * h;
        int pixel = y * 3 * w + x * 3;
        return uchar3_to_vec3(sprite[pixel], sprite[pixel+1], sprite[pixel+2]);
    }

    Texture::~Texture()
    {
        stbi_image_free(sprite);
    }
}
