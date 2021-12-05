#include "Shader.h"
namespace YYLB
{
    class PhongShader : public Shader
    {
        public:
            PhongShader() : Shader(){}
            PhongShader(Texture* tex):Shader(tex){}
            Vec4f vertex_shading(Vertex& v, Light* l) override;
            Vec3f fragment_shading(Triangle& t, Light* l) override;
    };
}