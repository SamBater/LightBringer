#include "Triangle.h"

using namespace ylb;

glm::vec3 ylb::Triangle::ComputeBarycentric(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P) {
    glm::vec3 e[2];
    for (int i = 2; i--;) {
        e[i][0] = C[i] - A[i];
        e[i][1] = B[i] - A[i];
        e[i][2] = A[i] - P[i];
    }
    auto u = glm::cross(e[0], e[1]);
    if (std::abs(u[2]) > 1e-2)
        return glm::vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return glm::vec3(-1, 1, 1);
}

void Triangle::ComputeBoundingBox() {
    float i = 1.f;
    bb.top = ylb::max3(vts[0].sy(), vts[1].sy(), vts[2].sy());
    bb.bot = ylb::min3(vts[0].sy(), vts[1].sy(), vts[2].sy());
    bb.right = ylb::max3(vts[0].sx(), vts[1].sx(), vts[2].sx());
    bb.left = ylb::min3(vts[0].sx(), vts[1].sx(), vts[2].sx());
}

float Triangle::interpolated_depth() {
    float depth_buffer = (bar.x * vts[0].sz() + bar.y * vts[1].sz() + bar.z * vts[2].sz()) * inv_w;
    return depth_buffer;
}

void Triangle::interpolated_uv(float &u, float &v) {
    u =  inv_w * (bar.x * vts[0].u() + bar.y * vts[1].u() + bar.z * vts[2].u());
    v =  inv_w * (bar.x * vts[0].v() + bar.y * vts[1].v() + bar.z * vts[2].v());
}

glm::vec3 Triangle::interpolated_world_position() {
    glm::vec3 pos;
    pos = (vts[0].world_coords * bar.x,
           vts[1].world_coords * bar.y,
           vts[2].world_coords * bar.z)
          * inv_w;
    return pos;
}

glm::vec3 Triangle::interpolated_world_normal() {
    glm::vec3 normal;
    normal = (vts[0].normal * bar.x + vts[1].normal * bar.y + vts[2].normal * bar.z) * inv_w;
    return normal;
}
