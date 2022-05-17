//
// Created by a on 2021/12/10.
//

#ifndef LIGHTBRINGER_SKYBOXSHADER_H
#define LIGHTBRINGER_SKYBOXSHADER_H

#include "Shader.h"
namespace ylb{
    class SkyBoxShader : public Shader{
    public:
        SkyBoxShader(CubeMap* cubeMap) : cube_map(cubeMap) { }
        CubeMap* cube_map;
        virtual glm::vec4 VertexShading(Vertex &v, const VertexShaderContext& contex) override;
        virtual glm::vec3 FragmentShading(Triangle &t, const FragmentShaderContext& contex) override;
    };
}

#endif //LIGHTBRINGER_SKYBOXSHADER_H
