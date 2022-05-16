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
    glm::vec4 VertexShading(Vertex &v, Light *l) override;
    glm::vec3 FragmentShading(Triangle &t, Light *l) override;
};
} // namespace ylb