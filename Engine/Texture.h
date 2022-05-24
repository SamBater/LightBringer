#pragma once
#include "glm/fwd.hpp"
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
        Texture(int w,int h);
        Texture(const Texture& texture){
            w = texture.w;
            h = texture.h;
            comp = texture.comp;
            sprite = new glm::vec3[w*h];
            for (int i = 0; i < w * h; i++)
                sprite[i] = texture.sprite[i];
        }
        void Fill(int x,int y,const glm::vec3& val){
            sprite[y*w+x] = val;
        }
        void Fill(const glm::vec3& fill_val){
            for(int i = 0 ; i < w * h ; i++)
                sprite[i] = fill_val;
        }
        virtual glm::vec3 tex2d(const float &u, const float &v);
        virtual ~Texture();
    };
}