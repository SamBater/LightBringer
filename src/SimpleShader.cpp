#include "Core/Shader/SimpleShader.h"
#include "Math/Matrix.h"
using namespace YYLB;

YYLB::Vec3f SimpleShader::shading(Vertex &v, Light *l)
{
    auto& normal = v.normal;
    auto& position_world = v.position_world;
    //纹理采样
    // t.interpolated_uv(u, v);
    // texture->tex2d(u, v, color);
    // Vec3f kd{color.x() / 255.f, color.y() / 255.f, color.z() / 255.f};
    Vec3f kd{1,1,1};

    //计算光照方向及衰减
    float attenuation = l->attenuation(position_world);
    Vec3f l_dir = l->LightDir(position_world);
    //归一化方向
    l_dir.normalized();
    normal.normalized();

    //Lambert term
    float nxl = dot_product(l_dir, normal);
    Vec3f L_diffuse = kd * std::max(0.f, nxl) * l->light_intense * attenuation;

    //Specular term
    // Vec3f viewDir = Vec3f{0, 0, 1} - position_world;
    // viewDir.normalized();
    // Vec3f h = (viewDir + l_dir);
    // h.normalized();
    // double nxh = dot_product(normal, h);
    // float p = 256;
    // Vec3f ks = {1.f, 1.f, 1.f};
    // Vec3f L_specular = ks * std::pow(std::max(0.0, nxh), p);
    // Vec3f L_diffuse = kd * (nxl + 1) * 0.5f * l->light_intense * attenuation;
    Vec3f ambient = {0.1f, 0.1f, 0.1f};
    Vec3f L = L_diffuse + ambient ;
    return L;
}

Vec3f SimpleShader::fragment_shading(Triangle &t, Light *l)
{

    //获得插值后的法线和位置
    // auto position_world = t.interpolated_world_position();
    // auto normal = t.interpolated_world_normal();

    //纹理采样
    // RGB color;
    // float u, v;
    // t.interpolated_uv(u, v);
    // texture->tex2d(u, v, color);
    // Vec3f kd{color.x() / 255.f, color.y() / 255.f, color.z() / 255.f};
    // Vec3f kd{1,1,1};

    //计算光照方向及衰减
    // float attenuation = l->attenuation(position_world);
    // Vec3f l_dir = l->LightDir(position_world);
    //归一化方向
    // l_dir.normalized();
    // normal.normalized();

    //Lambert term
    // float nxl = dot_product(l_dir, normal);
    // Vec3f L_diffuse = kd * std::max(0.f, nxl) * l->light_intense * attenuation;

    //Specular term
    // Vec3f viewDir = Vec3f{0, 0, 1} - position_world;
    // viewDir.normalized();
    // Vec3f h = (viewDir + l_dir);
    // h.normalized();
    // double nxh = dot_product(normal, h);
    // float p = 64;
    // Vec3f ks = {1.f, 1.f, 1.f};
    // Vec3f L_specular = ks * std::pow(std::max(0.0, nxh), p);
    // Vec3f ambient = {0.0f, 0.0f, 0.0f};
    // Vec3f L = L_diffuse + ambient;
    return  t.vts[0].color *t.cof.x() +  t.vts[1].color * t.cof.y() + t.vts[2].color * t.cof.z() ;
}
