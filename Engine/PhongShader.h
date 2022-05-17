#pragma once

#include "Shader.h"
namespace ylb {
class PhongShader : public Shader {
public:
    PhongShader() :
        Shader() {
    }
    PhongShader(Texture *tex) :
        Shader(tex) {
    }
    glm::vec4 VertexShading(Vertex &v, const VertexShaderContext& context) override;
    glm::vec3 FragmentShading(Triangle &t, const FragmentShaderContext& contex) override;
};
} // namespace ylb