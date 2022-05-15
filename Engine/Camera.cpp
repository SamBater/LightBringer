#include "Camera.h"
#include "glm/fwd.hpp"
namespace ylb {
Camera::Camera(const glm::vec3 &origin, const glm::vec3 &lookat,
               float fov_v, float _aspect_ratio,
               float _n, float _f) :
    Actor(origin),
    look_at(lookat), fov_Y(fov_v),
    aspect_ratio(_aspect_ratio), n(_n), f(_f) {
    t = std::tan(fov_Y / 2) * n;
    r = aspect_ratio * t;
    l = -r;
    b = -t;

    up = {0, 1, 0};
}

void Camera::set_perspective_matrix(glm::mat4 &mat) {
    mat = glm::mat4(0);
    mat[0][0] = 2 * n / (r - l);
    mat[0][2] = (l + r) / (l - r);
    mat[1][1] = 2 * n / (t - b);
    mat[1][2] = (b + t) / (b - t);
    mat[2][2] = (f + n) / (n - f);
    mat[2][3] = 2 * f * n / (f - n);
    mat[3][2] = 1;
}

void Camera::set_orthogonal_matrix(glm::mat4 &mat) {
    mat = glm::mat4(1);
    mat[0][0] = 2 / (r - l);
    mat[1][1] = 2 / (t - b);
    mat[2][2] = 2 / (n - f);
    mat[0][3] = -(r + l) / (r - l);
    mat[1][3] = -(t + b) / (t - b);
    mat[2][3] = -(f + n) / (n - f);
}

} // namespace ylb