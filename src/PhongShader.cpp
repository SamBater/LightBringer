#include "Core/Shader/PhongShader.h"
using namespace ylb;

glm::vec4 PhongShader::vertex_shading(Vertex &v, Light *l)
{
    glm::vec4 pos_h(v.position,1);

    //MVP变换 model->view->projection
    glm::mat4 mvp = model * view * projection;
    glm::vec4 ccv_pos = pos_h * mvp;
    v.l_pos = glm::vec4(v.position_world,1) * l->vp;

    return ccv_pos;
}

glm::vec3 PhongShader::fragment_shading(Triangle &t, Light *l)
{
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

    //计算阴影
    float visibility = 1.0f;
    if(l->shadow_map != nullptr)
    {
        glm::vec4 v_light_pos = t.vts[0].l_pos * t.cof.x  +
                            t.vts[1].l_pos * t.cof.y  +
                            t.vts[2].l_pos * t.cof.z  ;
        v_light_pos *= t.s;

        if(v_light_pos.x < -1 || v_light_pos.x > 1 ||
        v_light_pos.y < -1 || v_light_pos.y > 1 ||
        v_light_pos.z > 1 || v_light_pos.z < -1)
            visibility = 0.0;

        glm::vec4 one{1,1,1,1};
        v_light_pos = v_light_pos + one;
        v_light_pos *= 0.5f;
        float depth_lb = v_light_pos.z;

        float shadow_bias = 0.005f;
        float depth_sb = l->shadow_map->tex2d(v_light_pos.x,v_light_pos.y).z;

        if(depth_sb > depth_lb + shadow_bias)
            visibility = 0.5f;
    }

    //Lambert term
    float nxl = glm::dot(l_dir, normal);
    glm::vec3 L_diffuse = kd * (float)std::max(0.f, nxl) * attenuation * visibility;

    //Specular term
    glm::vec3 viewDir = *camPos - position_world;
    viewDir = glm::normalize(viewDir);
    glm::vec3 h = (viewDir + l_dir);
    h = glm::normalize(h);
    double nxh = glm::dot(normal, h);
    float p = 256;
    glm::vec3 ks = {1.f, 1.f, 1.f};
    glm::vec3 L_specular = ks * (float)std::pow(std::max(0.0, nxh), p) * visibility ;
    glm::vec3 ambient = {0.22f, 0.27f, 0.32f};
    glm::vec3 L = L_diffuse  + L_specular + ambient;
    return L;
}