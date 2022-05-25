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

    struct VertexShaderContext {
        const glm::mat4* model;
        const glm::mat4* view;
        const glm::mat4* project;
        const glm::vec3 *camPos;
        const Light *l;
        uint8_t nthVertex;
    };

    struct FragmentShaderContext {
        const glm::vec3* camPos;
        Light* l;
        FragmentShaderContext(const glm::vec3* camPos,Light* l) : camPos(camPos),l(l){}
    };

    class Shader
    {
    public:
        Shader() = default;
        virtual glm::vec4 VertexShading(Vertex &v, const VertexShaderContext& context) {return glm::vec4{1, 1, 1,1};}
        virtual glm::vec3 FragmentShading(Triangle &t, const FragmentShaderContext& context) {return glm::vec3{1, 1, 1};}
        virtual ~Shader()
        {

        }
    };
}