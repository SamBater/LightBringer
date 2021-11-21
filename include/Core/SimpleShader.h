#pragma once
#include "Shader.h"
#include "math/Common.h"
#include "math/Matrix.h"
#include <cmath>
#include "math/Triangle.h"
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