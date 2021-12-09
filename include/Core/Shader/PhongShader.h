#include "Shader.h"
namespace ylb
{
    class PhongShader : public Shader
    {
        public:
            PhongShader() : Shader(){}
            PhongShader(Texture* tex):Shader(tex){}
            glm::vec4 vertex_shading(Vertex& v, Light* l) override;
            glm::vec3 fragment_shading(Triangle& t, Light* l) override;
    };
}