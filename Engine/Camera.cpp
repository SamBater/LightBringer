#include "Camera.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
namespace ylb {

glm::mat4 Camera::GetViewMatrix() {
    //glm::mat4 view = glm::mat4(1);
    //auto g = look_at;
    //g = glm::normalize(g);
    //auto& t = up;

    //auto w = g * -1.0f;         w = glm::normalize(w);
    //auto u = glm::cross(t, w);   u = glm::normalize(u);
    //auto v = glm::cross(w, u);

    //view[0][0] = u.x; view[0][1] = u.y; view[0][2] = u.z;
    //view[1][0] = v.x; view[1][1] = v.y; view[1][2] = v.z;
    //view[2][0] = w.x; view[2][1] = w.y; view[2][2] = w.z;
    //view[0][3] = transform.WorldPosition().x * -1;
    //view[1][3] = transform.WorldPosition().y * -1;
    //view[2][3] = transform.WorldPosition().z * -1;

    auto r = glm::lookAt(transform.WorldPosition(), transform.WorldPosition() + Front, Up);
    r = glm::transpose(r);
    return r;
}

glm::mat4 Camera::GetPerspectiveMatrix() {
    glm::mat4 mat = glm::mat4(0);
    mat[0][0] = 2 * n / (r - l);
    mat[0][2] = (l + r) / (l - r);
    mat[1][1] = 2 * n / (t - b);
    mat[1][2] = (b + t) / (b - t);
    mat[2][2] = (f + n) / (n - f);
    mat[2][3] = 2 * f * n / (f - n);
    mat[3][2] = 1;
    
    return mat;
    //auto perspectiveMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
    //perspectiveMatrix = glm::transpose(perspectiveMatrix);
    //return perspectiveMatrix;
}

glm::mat4 Camera::GetOrthogonalMatrix() {
    glm::mat4 mat = glm::mat4(1);
    mat[0][0] = 2 / (r - l);
    mat[1][1] = 2 / (t - b);
    mat[2][2] = 2 / (n - f);
    mat[0][3] = -(r + l) / (r - l);
    mat[1][3] = -(t + b) / (t - b);
    mat[2][3] = -(f + n) / (n - f);
    return mat;
}

} // namespace ylb