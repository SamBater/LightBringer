#pragma once
#include "glm/glm.hpp"
#include "Common.h"
#include "algorithm"
namespace ylb
{
    class Texture
    {
    protected:
        unsigned char *sprite;
        int w;
        int h;
        int comp;

    public:
        Texture() = default;
        Texture(const char *file_name);
        Texture(unsigned char* data,int w,int h);
        virtual glm::vec3 tex2d(const float &tex_cord_x, const float &tex_cord_y);
        virtual ~Texture();
    };
}