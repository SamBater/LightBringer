#pragma once
#include "glm/glm.hpp"
#include "Actor.h"
#include "YLBSerializable.h"
namespace ylb
{
    struct Vertex
    {
        glm::vec3 position; //局部坐标
        glm::vec3 world_position;
        glm::vec3 sv_pos;   //屏幕空间坐标
        glm::vec2 tex_coord;//纹理坐标
        glm::vec3 normal;   //法线
        glm::vec3 tangent,bitangent;
        glm::vec4 ccv;
        float inv;
        Vertex() = default;
        Vertex(glm::vec3& position,glm::vec3 normal,glm::vec2 uv):
            position(position),normal(normal),tex_coord(uv){}
        
        const float &x() { return position.x; }
        const float &y() { return position.y; }
        const float &z() { return position.z; }
        float &sx() { return sv_pos.x; }
        float &sy() { return sv_pos.y; }
        float &sz() { return sv_pos.z; }
        const float &u() { return tex_coord.x; }
        const float &v() { return tex_coord.y; }

    };
    }