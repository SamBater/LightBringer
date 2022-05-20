#include "PhongShader.h"
using namespace ylb;

glm::vec4 PhongShader::VertexShading(Vertex &vt, const VertexShaderContext& contex)
{
    glm::vec4 pos_h(vt.position,1);
    glm::mat4 MVP = (*contex.model) * (*contex.view) * (*contex.project);
    glm::vec4 ccv_pos = pos_h * MVP;

    const auto& model = (glm::mat3(*contex.model));
    glm::vec3 T = glm::normalize(glm::vec3(model * glm::vec4(vt.tangent, 0.0)));
    //glm::vec3 B = glm::normalize(glm::vec3(model * glm::vec4(vt.bitangent, 0.0)));
    glm::vec3 N = glm::normalize(glm::vec3(model * glm::vec4(vt.normal, 0.0)));
    auto B = glm::normalize(glm::cross(T, N));
    TBN = glm::mat3(T, B, N);
    TBN = glm::transpose(TBN);

    return ccv_pos;
}

glm::vec3 PhongShader::FragmentShading(Triangle &t, const FragmentShaderContext& contex)
{
    auto* l = contex.l;
    auto* camPos = contex.camPos;
    auto position_world = t.interpolated_world_position();
    float u, v;
    t.interpolated_uv(u, v);
    auto normal = normal_map ? normal_map->tex2d(u, v) * 2.0f - glm::vec3(1, 1, 1) * TBN : t.interpolated_world_normal();
    
    auto kd = texture ? texture->tex2d(u,v) : glm::vec3{ 1,1,1 };
    //计算光照方向及衰减
    float attenuation = l->attenuation(position_world);
    glm::vec3 l_dir = l->LightDir(position_world) * TBN;
    //归一化方向
    l_dir = glm::normalize(l_dir);
    normal = glm::normalize(normal);


    //Lambert term
    float angle = std::max(0.0f, glm::dot(l_dir, normal));
    glm::vec3 L_diffuse = kd * angle * l->LightIntensity() * attenuation;

    //Specular term
    glm::vec3 viewDir = *camPos - position_world;
    viewDir = glm::normalize(viewDir);
    glm::vec3 h = (viewDir + l_dir);
    h = glm::normalize(h);
    double nxh = glm::dot(normal, h);
    float p = 256;
    glm::vec3 ks = {1.f, 1.f, 1.f};
    glm::vec3 L_specular = ks * (float)std::pow(std::max(0.0, nxh), p);
    glm::vec3 ambient(0.0, 0.0, 0);
    glm::vec3 L = L_diffuse  + L_specular + ambient;
    return L;
}