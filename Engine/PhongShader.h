#pragma once

#include "Shader.h"
namespace ylb {
class PhongShader : public Shader {
public:
    PhongShader() :
        Shader() {
    }
    PhongShader(Texture *tex,Texture* specular_map = nullptr,Texture* normal_map = nullptr) :
        Shader(tex) , specular_map(specular_map),normal_map(normal_map) {
    }
    glm::vec4 VertexShading(Vertex &v, const VertexShaderContext& context) override;
    glm::vec3 FragmentShading(Triangle &t, const FragmentShaderContext& contex) override;
private:
    Texture* specular_map;
    Texture* normal_map;
    glm::mat3 TBN;
};
} // namespace ylb