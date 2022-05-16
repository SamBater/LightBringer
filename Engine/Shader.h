#pragma once
#include "Light.h"
#include "glm/glm.hpp"
#include "Triangle.h"
#include "Texture.h"
#include "CubeMap.h"
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
        virtual glm::vec4 VertexShading(Vertex &v, Light *l) {return glm::vec4{1, 1, 1,1};}
        virtual glm::vec3 FragmentShading(Triangle &t, Light *l) {return glm::vec3{1, 1, 1};}
        virtual ~Shader()
        {
            delete texture;
        }
    };
}