#pragma once

#include "Shader.h"
namespace ylb {
class PhongShader : public Shader {
public:
    PhongShader() :
        Shader() {
    }
    PhongShader(Texture *tex,Texture* normal_map = nullptr) :
        Shader(tex) , normal_map(normal_map) {
    }
    glm::vec4 VertexShading(Vertex &v, const VertexShaderContext& context) override;
    glm::vec3 FragmentShading(Triangle &t, const FragmentShaderContext& contex) override;
private:
    Texture* normal_map;
};
} // namespace ylb