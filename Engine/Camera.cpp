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
    auto perspectiveMatrix = glm::perspective(glm::radians(fov_Y), aspect_ratio, 0.1f, f);
    return perspectiveMatrix;
}

glm::mat4 Camera::GetOrthogonalMatrix() {
    return glm::ortho<float>(-10, 10, -10, 10, -20, 100);
}

} // namespace ylb