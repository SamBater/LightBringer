#include "Camera.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <cstdlib>
#include <stdlib.h>
namespace ylb {

glm::mat4 Camera::GetViewMatrix() {
    auto r = glm::lookAt(transform.WorldPosition(), transform.WorldPosition() + Front, Up);
    r = glm::transpose(r);
    return r;
}

glm::mat4 Camera::GetPerspectiveMatrix() {
    //glm::mat4 mat = glm::mat4(0);
    //mat[0][0] = 1;
    //mat[1][1] = 1;
    //mat[2][2] = 1.0 * (f + n) / (n - f);
    //mat[2][3] = 2.0 * f * n / (n - f);
    //mat[3][2] = -1;
    //return mat;

    auto perspectiveMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    perspectiveMatrix = glm::transpose(perspectiveMatrix);
    return perspectiveMatrix;
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