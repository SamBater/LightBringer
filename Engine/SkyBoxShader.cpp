//
// Created by a on 2021/12/10.
//

#include "SkyBoxShader.h"


glm::vec4 ylb::SkyBoxShader::VertexShading(ylb::Vertex &v, const VertexShaderContext& context) {
    v.tex_coord = v.position;
    auto view = glm::mat4 (glm::mat3(*context.view));
    glm::vec4 glPosition = glm::vec4(v.position, 1.0) * view * *context.project;
    return glPosition;
}

glm::vec3 ylb::SkyBoxShader::FragmentShading(ylb::Triangle &t, const FragmentShaderContext& context) {

    //glm::vec3 dir = (t.vts[0].tex_coord * t.cof.x +
    //                 t.vts[1].tex_coord * t.cof.y +
    //                    t.vts[2].tex_coord * t.cof.z);
    //dir *= t.s;
    //return cube_map->tex3d(dir);
    return glm::vec3(1, 1, 1);
}
