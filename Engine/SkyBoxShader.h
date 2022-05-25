#ifndef LIGHTBRINGER_SKYBOXSHADER_H
#define LIGHTBRINGER_SKYBOXSHADER_H

#include "Shader.h"
namespace ylb{
    class SkyBoxShader : public Shader{
    public:
        SkyBoxShader(CubeMap* cubeMap) : cube_map(cubeMap) { }
        virtual glm::vec4 VertexShading(Vertex &v, const VertexShaderContext& contex) override;
        virtual glm::vec3 FragmentShading(Triangle &t, const FragmentShaderContext& contex) override;

    private:
        CubeMap *cube_map;
        glm::vec3 uv[3];
    };
}

#endif //LIGHTBRINGER_SKYBOXSHADER_H
