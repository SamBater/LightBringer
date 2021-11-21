#ifndef YYLB_VERTEXS_H
#define YYLB_VERTEXS_H
#include <math/Matrix.h>
#include "Core/Actor.h"
#include "Core/Light.h"
#include "math/Matrix.h"
namespace YYLB
{
    class Transformer;
    class Triangle;
    class Vertex : public Actor
    {
    public:
        Vec3f position; //局部坐标
        Vec3f sv_pos;   //屏幕空间坐标
        Vec2f uv;       //纹理坐标
        Vec3f normal;   //法线
        Vec3f color;
        friend class Transformer;
        friend class Triangle;
        // friend class YYLB::Shader;
        // friend class YYLB::SimpleShader;
        float inv;

    // public:
        Vertex(Vec3f &&position, Vec3f&& normal,Vec2f &&uv);
        Vertex(Vec3f &position, Vec3f& normal,Vec2f &uv);
        inline const float &x() { return position.x(); }
        inline const float &y() { return position.y(); }
        inline const float &z() { return position.z(); }
        inline float &sx() { return sv_pos.x(); }
        inline float &sy() { return sv_pos.y(); }
        inline float &sz() { return sv_pos.z(); }
        inline const float &u() { return uv.x(); }
        inline const float &v() { return uv.y(); }
        inline void set_uv(float u, float v)
        {
            uv = {u, v};
        }
        inline void set_normal(Vec3f &n)
        {
            normal = n;
        }
        inline void set_pos(float x,float y,float z)
        {
            position = {x,y,z};
        }
    };
}
#endif