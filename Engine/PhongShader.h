#pragma once

#include "Shader.h"
namespace ylb {
class PhongShader : public Shader {
public:
    PhongShader() :
        Shader() {
    }
    PhongShader(Texture *diffuse_map,Texture* specular_map = nullptr,Texture* normal_map = nullptr) :
        diffuse_map(diffuse_map) , specular_map(specular_map),normal_map(normal_map) {
    }
    glm::vec4 VertexShading(Vertex &v, const VertexShaderContext& context) override;
    glm::vec3 FragmentShading(Triangle &t, const FragmentShaderContext& contex) override;
private:
    Texture* diffuse_map;
    Texture* specular_map;
    Texture* normal_map;
    glm::mat3 TBN;
};
} // namespace ylb