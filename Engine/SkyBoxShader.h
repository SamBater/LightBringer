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
        virtual glm::vec4 VertexShading(Vertex &v, Light *l) override;
        virtual glm::vec3 FragmentShading(Triangle &t, Light *l) override;
    };
}

#endif //LIGHTBRINGER_SKYBOXSHADER_H
