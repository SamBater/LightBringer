#include "Camera.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <cstdlib>
#include <stdlib.h>
namespace ylb {

glm::mat4 Camera::GetViewMatrix() {
    auto r = glm::lookAt(transform.WorldPosition(), transform.WorldPosition() + Front, WorldUp);
    return r;
}

glm::mat4 Camera::GetProjectionMatrix() {
    if (mode == PROJECTION_MODE::PERSPECTIVE)
        return GetPerspectiveMatrix();
    return GetOrthogonalMatrix();
}

glm::mat4 Camera::GetPerspectiveMatrix() {
    auto perspectiveMatrix = glm::perspective(glm::radians(fov_Y), aspect_ratio, n, f);
    return perspectiveMatrix;
}

glm::mat4 Camera::GetOrthogonalMatrix() {
    //return glm::ortho(0, 600, 0, 600);
    auto mat = glm::mat4(1);
    mat[0][0] = 2.0 / (r - l);
    mat[1][1] = 2.0 / (t - b);
    mat[2][2] = 2.0 / (n - f);
    mat[3][0] = -(r + l) / 2.0;
    mat[3][1] = -(t + b) / 2.0;
    mat[3][2] = -(f + n) / 2.0;
    return mat;
}

} // namespace ylb