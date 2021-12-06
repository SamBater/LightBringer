#include "Core/Shader/PhongShader.h"
#include "Math/Vertex.h"
using namespace YYLB;

Vec4f PhongShader::vertex_shading(Vertex &v, Light *l)
{
    Vec4f pos_h{v.x(),v.y(),v.z(),1};

    //MVP变换 model->view->projection
    Vec4f ccv_pos = mvp * pos_h;
    v.l_pos = l->lvp * pos_h;

    //裁剪
    if (ccv_pos.x() >= ccv_pos.w() || ccv_pos.x() <= -ccv_pos.w())
        return vaild_pos;
    if (ccv_pos.y() >= ccv_pos.w() || ccv_pos.y() <= -ccv_pos.w())
        return vaild_pos;
    if(ccv_pos.z() >= ccv_pos.w() || ccv_pos.z() <= -ccv_pos.w())
        return vaild_pos;


    //透视除法 CLIP->NDC
    float w = ccv_pos.w();
    ccv_pos /= w;
    v.sz() = ccv_pos.z() * v.inv;

    v.inv = 1.0f / w;
    v.set_uv(v.u() * v.inv, v.v() * v.inv);
    v.normal = v.normal * v.inv;
    v.position_world = v.position_world * v.inv;

    return ccv_pos;
}

Vec3f PhongShader::fragment_shading(Triangle &t, Light *l)
{
    auto normal = t.interpolated_world_normal();
    auto position_world = t.interpolated_world_position();
    //纹理采样
    float u, v;
    t.interpolated_uv(u, v);
    auto kd = texture ? texture->tex2d(u, v) : Vec3f{ 1,1,1 };

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
    float p = 256;
    Vec3f ks = {1.f, 1.f, 1.f};
    Vec3f L_specular = ks * std::pow(std::max(0.0, nxh), p);
    Vec3f ambient = {0.2f, 0.2f, 0.2f};
    Vec3f L = L_diffuse  + L_specular + ambient;
    return L;
}