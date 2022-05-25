#include "SkyBoxShader.h"

glm::vec4 ylb::SkyBoxShader::VertexShading(ylb::Vertex &v, const VertexShaderContext& context) {
    uv[context.nthVertex] = v.model_coords;
    glm::mat4 view = glm::mat4(*context.view);
    glm::vec4 glPosition = *context.project * view * glm::vec4(v.model_coords,1);
    //glPosition.z = glPosition.w;
    return glPosition;
}

glm::vec3 ylb::SkyBoxShader::FragmentShading(ylb::Triangle &t, const FragmentShaderContext& context) {

    glm::vec3 dir = t.bar.x * uv[0] + t.bar.y * uv[1] + t.bar.z * uv[2];
    //dir *= t.inv_w;
    return cube_map->tex3d(dir);
}
