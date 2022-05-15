#pragma once
#include "glm/glm.hpp"
#include "Actor.h"
namespace ylb
{
    class Transformer;
    class Triangle;
    class Vertex : public Actor
    {
    public:
        glm::vec3 position; //局部坐标
        glm::vec3 sv_pos;   //屏幕空间坐标
        glm::vec3 tex_coord;       //纹理坐标
        glm::vec3 normal;   //法线
        glm::vec3 color;
        glm::vec4 ccv;
        glm::vec4 l_pos;    //光源空间坐标
        friend class Transformer;
        friend class Triangle;
        float inv;

    // public:
        Vertex(glm::vec3 &&position, glm::vec3 &&normal,glm::vec3 &&uv) : position(position), normal(normal),tex_coord(uv){}
        Vertex() = default;
        Vertex(glm::vec3 &position, glm::vec3& normal, glm::vec3 &uv) : position(position), normal(normal),tex_coord(uv){}
        Vertex(glm::vec3 &&position, glm::vec3 &&normal,glm::vec2 &&uv) : position(position), normal(normal),tex_coord(uv,0){}
        Vertex(glm::vec3 &position, glm::vec3& normal, glm::vec2 &uv) : position(position), normal(normal),tex_coord(uv,0){}
        inline const float &x() { return position.x; }//模型空间坐标
        inline const float &y() { return position.y; }
        inline const float &z() { return position.z; }
        inline float &sx() { return sv_pos.x; }
        inline float &sy() { return sv_pos.y; }
        inline float &sz() { return sv_pos.z; }
        inline const float &u() { return tex_coord.x; }
        inline const float &v() { return tex_coord.y; }
    };
}