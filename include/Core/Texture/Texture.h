#pragma once
#include "glm/glm.hpp"
#include "Math/Common.h"
#include "algorithm"
namespace YYLB
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
        Texture(unsigned char* data,int w,int h) :sprite(data),w(w),h(h),comp(3){}
        virtual glm::vec3 tex2d(const float &tex_cord_x, const float &tex_cord_y);
        virtual ~Texture();
    };
}