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
        virtual glm::vec4 vertex_shading(Vertex &v, Light *l);
        virtual glm::vec3 fragment_shading(Triangle &t, Light *l);
    };
}

#endif //LIGHTBRINGER_SKYBOXSHADER_H
