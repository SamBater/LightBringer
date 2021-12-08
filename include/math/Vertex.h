#pragma once
#include "glm/glm.hpp"
#include "Core/Scene/Actor.h"
namespace YYLB
{
    class Transformer;
    class Triangle;
    class Vertex : public Actor
    {
    public:
        glm::vec3 position; //局部坐标
        glm::vec3 sv_pos;   //屏幕空间坐标
        glm::vec2 uv;       //纹理坐标
        glm::vec3 normal;   //法线
        glm::vec3 color;
        glm::vec4 l_pos;    //光源空间坐标
        friend class Transformer;
        friend class Triangle;
        float inv;

    // public:
        Vertex(glm::vec3 &&position, glm::vec3 &&normal,glm::vec2 &&uv) : position(position), normal(normal),uv(uv){}

        Vertex(glm::vec3 &position, glm::vec3& normal, glm::vec2 &uv) : position(position), normal(normal),uv(uv){}
        inline const float &x() { return position.x; }//模型空间坐标
        inline const float &y() { return position.y; }
        inline const float &z() { return position.z; }
        inline float &sx() { return sv_pos.x; }
        inline float &sy() { return sv_pos.y; }
        inline float &sz() { return sv_pos.z; }
        inline const float &u() { return uv.x; }
        inline const float &v() { return uv.y; }
        inline void set_uv(float u, float v)
        {
            uv = {u, v};
        }
        inline void set_normal(glm::vec3 &n)
        {
            normal = n;
        }
        inline void set_pos(float x,float y,float z)
        {
            position = {x,y,z};
        }
    };
}