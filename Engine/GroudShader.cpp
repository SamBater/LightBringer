#include "GroudShader.h"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"

glm::vec4 ylb::GroudShader::VertexShading(Vertex &v,
                                          const VertexShaderContext &context) {
    glm::vec4 pos_h(v.position, 1);

    // MVP
    auto MVP = (*context.model) * (*context.view) * (*context.project);
    glm::vec4 ccv_pos = pos_h * MVP;

    const auto& model = *context.model;
    glm::vec3 T = normalize(glm::vec3(model * glm::vec4(v.tangent,   0.0)));
    glm::vec3 B = normalize(glm::vec3(model * glm::vec4(v.bitangent, 0.0)));
    glm::vec3 N = normalize(glm::vec3(model * glm::vec4(v.normal,    0.0)));
    TBN = glm::mat3(T, B, N);
    TBN = glm::transpose(TBN);
    return ccv_pos;
}

glm::vec3
ylb::GroudShader::FragmentShading(Triangle &t,
                                  const FragmentShaderContext &context) {
    float u,v;
    t.interpolated_uv(u,v);
    auto normal = t.interpolated_world_normal();
    float light_intensity = 0;
    //for(int i = 0 ; i < 3 ; i++)
    //  light_intensity+= varying_color[i] * t.cof[i];
    
    auto diffuse = diffuse_map->tex2d(u, v);
    if (normal_map)
    {
        normal = normal_map->tex2d(u, v) * 2.0f - glm::vec3(1,1,1);
        normal = glm::normalize(normal *TBN);
        // return normal * 0.5f + glm::vec3(0.5,0.5,0.5);
    }

    glm::vec3 l = glm::vec4(context.l->LightDir(normal), 0);
    l = glm::normalize(l);
    light_intensity = std::max(0.0f, glm::dot(normal,  l));
    return diffuse * light_intensity;
}
