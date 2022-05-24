#include "PhongShader.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
using namespace ylb;

glm::vec4 PhongShader::VertexShading(Vertex &vt, const VertexShaderContext& contex)
{
    glm::vec4 pos_h(vt.position,1);
    glm::mat4 MVP = (*contex.project) * (*contex.view) * (*contex.model);
    glm::vec4 ccv_pos = MVP * pos_h;

    const auto& model = (glm::mat3(*contex.model));
    glm::vec3 T = glm::normalize(glm::vec3(model * glm::vec4(vt.tangent, 0.0)));
    glm::vec3 N = glm::normalize(glm::vec3(model * glm::vec4(vt.normal, 0.0)));
    // T = normalize(T - dot(T, N) * N);
    auto B = cross(N, T);
    TBN = glm::mat3(T, B, N);
    TBN = glm::transpose(TBN);

    return ccv_pos;
}

glm::vec3 PhongShader::FragmentShading(Triangle &t, const FragmentShaderContext& contex)
{
    auto* l = contex.l;
    auto camPos = *contex.camPos;
    auto position_world = t.interpolated_world_position();
    float u, v;
    t.interpolated_uv(u, v);

    // auto normal = normal_map ? TBN * (normal_map->tex2d(u, v) * 2.0f - glm::vec3(1, 1, 1)) : t.interpolated_world_normal();
    auto normal = t.interpolated_world_normal();

    
    //计算光照方向及衰减
    float attenuation = l->attenuation(position_world);
    glm::vec3 l_dir = l->LightDir(position_world);
    //归一化方向
    l_dir = glm::normalize(l_dir);
    normal = glm::normalize(normal);

    float angle = std::max(0.0f, glm::dot(l_dir, normal));

    //Lambert term
    auto kd = texture ? texture->tex2d(u,v) : glm::vec3{ 1,1,1 };
    glm::vec3 L_diffuse = kd * angle * l->LightIntensity() * attenuation;

    //Specular term
    glm::vec3 viewDir = glm::normalize(camPos - position_world);
    glm::vec3 h = glm::normalize(viewDir + l_dir);
    float nxh = std::max(0.0f, glm::dot(normal, h));

    float smothness = 512;
    auto ks = specular_map ? specular_map->tex2d(u,v).r : 0.5f;
    glm::vec3 L_specular = ks * (float)std::pow(nxh, smothness) * glm::vec3(1,1,1);
    glm::vec3 ambient(0.0, 0.0, 0);

    glm::vec3 L = L_diffuse + L_specular + ambient;
    for(int i = 0 ; i < 3 ; i++)
        L[i] = glm::clamp(L[i],0.0f,1.0f);
    return L;
}