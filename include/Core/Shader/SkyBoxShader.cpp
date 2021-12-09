//
// Created by a on 2021/12/10.
//

#include "SkyBoxShader.h"


glm::vec4 ylb::SkyBoxShader::vertex_shading(ylb::Vertex &v, ylb::Light *l) {
    v.tex_coord = v.position;
    view[0][3] = view[1][3] = view[2][3] = 0;
    glm::vec4 glPosition = glm::vec4(v.position,1.0) * view * projection;
    return glPosition;
}

glm::vec3 ylb::SkyBoxShader::fragment_shading(ylb::Triangle &t, ylb::Light *l) {

    glm::vec3 dir = (t.vts[0].tex_coord * t.cof.x +
                     t.vts[1].tex_coord * t.cof.y +
                        t.vts[2].tex_coord * t.cof.z);
    dir *= t.s;
    return cube_map->tex3d(dir);
}
