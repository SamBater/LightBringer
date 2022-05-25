#pragma once
#include "glm/glm.hpp"
#include "Actor.h"
#include "YLBSerializable.h"
namespace ylb
{
    struct Vertex
    {
        glm::vec3 model_coords;
        glm::vec3 world_coords;
        glm::vec4 screen_coords;
        glm::vec2 texture_coords;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec4 light_coords;
        glm::vec4 ccv;
        Vertex() = default;
        Vertex(glm::vec3& model_coords,glm::vec3 normal,glm::vec2 uv):
            model_coords(model_coords),normal(normal),texture_coords(uv){}
        
        const float &x() { return model_coords.x; }
        const float &y() { return model_coords.y; }
        const float &z() { return model_coords.z; }
        float &sx() { return screen_coords.x; }
        float &sy() { return screen_coords.y; }
        float &sz() { return screen_coords.z; }
        const float &u() { return texture_coords.x; }
        const float &v() { return texture_coords.y; }

    };
    }