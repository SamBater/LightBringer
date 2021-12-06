#include "Core/Texture/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"


namespace YYLB
{
    Texture::Texture(char const *file_name)
    {
        stbi__vertically_flip_on_load = 1;
        sprite = stbi_load(file_name, &w, &h, &comp, 0);
    }

    Vec3f uchar3_to_vec3(unsigned char r, unsigned char g, unsigned char b)
    {
        static float inv = 1 / 255.f;
        return Vec3f{ r * inv,g * inv,b * inv };
    }

    Vec3f Texture::tex2d(const float &tex_cord_x, const float &tex_cord_y)
    {
//        nearest
        int x = tex_cord_x * w;
        int y = tex_cord_y * h;
        int pixel = y * 3 * w + x * 3;
        return uchar3_to_vec3(sprite[pixel], sprite[pixel+1], sprite[pixel+2]);
        auto calc_index = [&](int x,int y)
        {
            return y*w*comp + x * comp;
        };
        float cx = tex_cord_x * w;
        float cy = tex_cord_y * h;

        float t1 = cx - std::floor(cx);
        float t2 = cy - std::floor(cy);
        float t3 = 1 - cx;
        float t4 = 1 - cy;

        using YYLB::clamp;
        int p1 = calc_index(cx+t1,cy+t2);
        p1 = clamp(p1,0,h*w*comp);Vec3f c1 = uchar3_to_vec3(sprite[p1],sprite[p1+1],sprite[p1+2]);
        int p2 = calc_index(cx+t1,cy-t2);p2 = clamp(p2,0,h*w*comp);Vec3f c2 = uchar3_to_vec3(sprite[p2],sprite[p2+1],sprite[p2+2]);
        int p3 = calc_index(cx-t3,cy-t4);p3 = clamp(p3,0,h*w*comp);Vec3f c3 = uchar3_to_vec3(sprite[p3],sprite[p3+1],sprite[p3+2]);
        int p4 = calc_index(cx+t3,cy-t4);p4 = clamp(p4,0,h*w*comp);Vec3f c4 = uchar3_to_vec3(sprite[p4],sprite[p4+1],sprite[p4+2]);

        Vec3f l1 = lerp(c1,c2,t1);
        Vec3f l2 = lerp(c3,c4,t3);

        return (l1+l2) / 2;

    }

    Texture::~Texture()
    {
        stbi_image_free(sprite);
    }
}
