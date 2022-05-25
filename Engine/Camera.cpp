#include "Camera.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <cstdlib>
#include <stdlib.h>
namespace ylb {

glm::mat4 Camera::GetViewMatrix() {
    auto viewMatrix = glm::lookAt(transform.WorldPosition(), transform.WorldPosition() + Front, WorldUp);
    return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() {
    if (mode == PROJECTION_MODE::PERSPECTIVE)
        return GetPerspectiveMatrix();
    return GetOrthogonalMatrix();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    auto Position = transform.WorldPosition();
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == Camera_Movement::Up)
        Position += Up * velocity;
    if (direction == Camera_Movement::Down)
        Position -= Up * velocity;
    transform.SetPosition(Position);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraInfo();
}

void Camera::UpdateCameraInfo() {
    //// calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 Camera::GetPerspectiveMatrix() {
    auto perspectiveMatrix = glm::perspective(glm::radians(fov_Y), aspect_ratio, n, f);
    return perspectiveMatrix;
}

glm::mat4 Camera::GetOrthogonalMatrix() {
    return glm::ortho<float>(-20, 20, -20, 20, -20, 100);
}

} // namespace ylb