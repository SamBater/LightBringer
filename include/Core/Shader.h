#pragma once
#include "Core/Light.h"
#include "Core/Texture.h"
#include <math/Vertex.h>
namespace YYLB
{
    class Shader
    {
    protected:
       Texture * texture; 
    public:
        Shader(){}
        Shader(Texture* t) : texture(t) {}
        virtual Vec3f shading(Vertex& v,Light* l);
        virtual Vec3f fragment_shading(Triangle& t,Light* l);
    };
}