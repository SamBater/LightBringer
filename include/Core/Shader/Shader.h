#pragma once
#include "Core/Light/Light.h"
#include "glm/glm.hpp"
#include "Math/Triangle.h"
#include "Core/Texture/Texture.h"
#include "Core/Texture/CubeMap.h"
#include <cmath>
#include <algorithm>
namespace ylb
{
    class Shader
    {
    public:
        Texture *texture;
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        static glm::vec3* camPos;
        Shader() { texture = nullptr; }
        Shader(Texture *t) : texture(t) {}
        virtual glm::vec4 vertex_shading(Vertex &v, Light *l) {return glm::vec4{1, 1, 1,1};}
        virtual glm::vec3 fragment_shading(Triangle &t, Light *l) {return glm::vec3{1, 1, 1};}
        virtual ~Shader()
        {
            delete texture;
        }
    };
}