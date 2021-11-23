#pragma once
#include "Shader.h"
#include "Math/Common.h"
#include <cmath>
#include "Math/Triangle.h"
namespace YYLB{
    class GouraudShader : public Shader
    {
    private:
        /* data */
    public:
        GouraudShader() : Shader(){}
        GouraudShader(Texture* tex):Shader(tex){}
        virtual Vec3f shading(Vertex& v,Light* l);
        virtual Vec3f fragment_shading(Triangle& t,Light* l);
    };
}