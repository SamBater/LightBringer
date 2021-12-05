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
        Matrix4f mvp;
    public:
        static Vec3f* camPos;
        Shader() { texture = nullptr; }
        Shader(Texture *t) : texture(t) {}
        virtual Vec4f vertex_shading(Vertex &v, Light *l) {return Vec4f{1, 1, 1,1};}
        virtual Vec3f fragment_shading(Triangle &t, Light *l) {return Vec3f{1, 1, 1};}
        virtual ~Shader()
        {
            delete texture;
        }
        void setMVP(Matrix4f& mat_mvp)
        {
            mvp = mat_mvp;
        }
    };
}