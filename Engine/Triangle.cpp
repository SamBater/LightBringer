#include "Triangle.h"

using namespace ylb;

void Triangle::ready_rasterization() {
    float i = 1.f;
    area = det(vts[0].sx(), vts[1].sx(), vts[2].sx(),
               vts[0].sy(), vts[1].sy(), vts[2].sy(),
               i, i, i);
    bb.top = ylb::max3(vts[0].sy(), vts[1].sy(), vts[2].sy());
    bb.bot = ylb::min3(vts[0].sy(), vts[1].sy(), vts[2].sy());
    bb.right = ylb::max3(vts[0].sx(), vts[1].sx(), vts[2].sx());
    bb.left = ylb::min3(vts[0].sx(), vts[1].sx(), vts[2].sx());
}

float Triangle::interpolated_depth() {
    float depth = (cof.x * vts[0].sz() + cof.y * vts[1].sz() + cof.z * vts[2].sz()) * s;
    return depth;
}

void Triangle::interpolated_uv(float &u, float &v) {
    u = s * (cof.x * vts[0].u() + cof.y * vts[1].u() + cof.z * vts[2].u());
    v = s * (cof.x * vts[0].v() + cof.y * vts[1].v() + cof.z * vts[2].v());
}

glm::vec3 Triangle::interpolated_world_position() {
    glm::vec3 pos;
    pos = (vts[0].world_position * cof.x,
           vts[1].world_position * cof.y,
           vts[2].world_position * cof.z)
          * s;
    return pos;
}

glm::vec3 Triangle::interpolated_world_normal() {
    glm::vec3 normal;
    normal = (vts[0].normal * cof.x + vts[1].normal * cof.y + vts[2].normal * cof.z) * s;
    return normal;
}
