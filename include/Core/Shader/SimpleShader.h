#pragma once
#include "Shader.h"
#include "Math/Common.h"
#include <cmath>
#include "Math/Triangle.h"
namespace YYLB{
    class SimpleShader : public Shader
    {
    private:
        /* data */
    public:
        SimpleShader() : Shader(){}
        SimpleShader(Texture* tex):Shader(tex){}
        virtual Vec3f shading(Vertex& v,Light* l);
        virtual Vec3f fragment_shading(Triangle& t,Light* l);
    };
}