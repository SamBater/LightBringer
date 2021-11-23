#include "Shader.h"
namespace YYLB
{
    class PhongShader : public Shader
    {
        public:
            PhongShader() : Shader(){}
            PhongShader(Texture* tex):Shader(tex){}
            virtual Vec3f shading(Vertex& v,Light* l);
            virtual Vec3f fragment_shading(Triangle& t,Light* l);
    };
}