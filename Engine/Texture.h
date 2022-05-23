#pragma once
#include "glm/glm.hpp"
#include "Common.h"
#include "algorithm"
namespace ylb
{
    class Texture
    {
    protected:
        glm::vec3* sprite;
        int w;
        int h;
        int comp;

    public:
        Texture() = default;
        Texture(const char *file_name , int maxComp = 255);
        Texture(float* data,int w,int h,int comp,float maxcomp);
        virtual glm::vec3 tex2d(const float &u, const float &v);
        virtual ~Texture();
    };
}