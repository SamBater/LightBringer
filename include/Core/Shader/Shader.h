#pragma once
#include "Core/Light/Light.h"
#include "Math/Matrix.h"
#include "Math/Triangle.h"
#include "Core/Texture/Texture.h"
namespace YYLB
{
    class Shader
    {
    protected:
        Texture *texture;

    public:
        Shader() {}
        Shader(Texture *t) : texture(t) {}
        virtual Vec3f shading(Vertex &v, Light *l) = 0;
        virtual Vec3f fragment_shading(Triangle &t, Light *l) = 0;
        virtual ~Shader()
        {
            delete texture;
        }
    };
}