#include "PhongShader.h"
using namespace ylb;

glm::vec4 PhongShader::VertexShading(Vertex &v, const VertexShaderContext& contex)
{
    glm::vec4 pos_h(v.position,1);

    //MVP变换
    glm::mat4 MVP = (*contex.model) * (*contex.view) * (*contex.project);
    glm::vec4 ccv_pos = pos_h * MVP;

    return ccv_pos;
}

glm::vec3 PhongShader::FragmentShading(Triangle &t, const FragmentShaderContext& contex)
{
    auto* l = contex.l;
    auto* camPos = contex.camPos;
    auto normal = t.interpolated_world_normal();
    auto position_world = t.interpolated_world_position();
    //纹理采样
    float u, v;
    t.interpolated_uv(u, v);
    auto kd = texture ? texture->tex2d(u, v) : glm::vec3{ 1,1,1 };

    //计算光照方向及衰减
    float attenuation = l->attenuation(position_world);
    glm::vec3 l_dir = l->LightDir(position_world);
    //归一化方向
    l_dir = glm::normalize(l_dir);
    normal = glm::normalize(normal);


    //Lambert term
    float nxl = glm::dot(l_dir, normal);
    glm::vec3 L_diffuse = kd * (float)std::max(0.f, nxl) * l->LightIntensity() * attenuation;

    //Specular term
    glm::vec3 viewDir = *camPos - position_world;
    viewDir = glm::normalize(viewDir);
    glm::vec3 h = (viewDir + l_dir);
    h = glm::normalize(h);
    double nxh = glm::dot(normal, h);
    float p = 2048;
    glm::vec3 ks = {1.f, 1.f, 1.f};
    glm::vec3 L_specular = ks * (float)std::pow(std::max(0.0, nxh), p);
    glm::vec3 L = L_diffuse  + L_specular;
    return L;
}