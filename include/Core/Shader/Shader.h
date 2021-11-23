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
        Vec3f* camPos;
        Shader() {}
        Shader(Texture *t) : texture(t) {}
        virtual Vec3f shading(Vertex &v, Light *l) {return Vec3f{1,1,1};}
        virtual Vec3f fragment_shading(Triangle &t, Light *l) {return Vec3f{1,1,1};}
        virtual ~Shader()
        {
            delete texture;
        }
    };
}