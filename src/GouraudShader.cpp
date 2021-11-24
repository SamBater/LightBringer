#include "Core/Shader/GouraudShader.h"
#include "Math/Matrix.h"
using namespace YYLB;

YYLB::Vec3f GouraudShader::shading(Vertex &v, Light *l)
{
    auto& normal = v.normal;
    auto& position_world = v.position_world;

    Vec3f kd{1.f,1.f,1.f};

    //计算光照方向及衰减
    float attenuation = l->attenuation(position_world);
    Vec3f l_dir = l->LightDir(position_world);
    //归一化方向
    l_dir.normalized();
    normal.normalized();

    //Lambert term
    float nxl = dot_product(l_dir, normal);
    Vec3f L_diffuse = kd * std::max(0.f, nxl) * attenuation;

    //Specular term
    Vec3f viewDir = *camPos - position_world;
    viewDir.normalized();
    Vec3f h = (viewDir + l_dir);
    h.normalized();
    double nxh = dot_product(normal, h);
    float p = 512;
    Vec3f ks = {1, 1, 1};
    Vec3f L_specular = ks * std::pow(std::max(0.0, nxh), p);
    Vec3f ambient = {0.0f, 0.0f, 0.0f};
    Vec3f L = L_diffuse + L_specular + ambient;
    return L * v.inv;
}

Vec3f GouraudShader::fragment_shading(Triangle &t, Light *l)
{
    auto& vts = t.vts;
    auto& cof = t.cof; 
    float s = 1 / (cof.x() * vts[0].inv + cof.y() * vts[1].inv + cof.z() * vts[2].inv);
    return  (t.vts[0].color *t.cof.x() +  t.vts[1].color * t.cof.y() + t.vts[2].color * t.cof.z()) * s ;
}
