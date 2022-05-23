#ifndef YYLB_CAMERA
#define YYLB_CAMERA
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "Actor.h"
#include "YLBSerializable.h"
#include "GLFW/glfw3.h"
namespace ylb {
class Transformer;

enum class PROJECTION_MODE { PERSPECTIVE,
                             ORTHOGONAL };

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    Up,
    Down
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera : public Actor, public YLBSerializable {
private:
    float fov_Y;
    float aspect_ratio;
    float n, f, l, r, b, t;
    friend class Renderer;

public:
    enum PROJECTION_MODE mode;
    // camera Attributes
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);
    // euler Angles
    float Yaw = YAW;
    float Pitch = PITCH;
    // camera options
    float MovementSpeed = SPEED;
    float MouseSensitivity = SENSITIVITY;
    float Zoom;
    Camera() = default;
    glm::mat4 GetPerspectiveMatrix();
    glm::mat4 GetOrthogonalMatrix();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
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

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
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
        UpdateProjectionInfo();
    }

    void UpdateProjectionInfo() {
        t = std::tan(glm::radians(fov_Y) / 2) * n;
        r = aspect_ratio * t;
        l = -r;
        b = -t;
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }

    virtual void DeSerilization(const json11::Json &json) override {
        transform.SetPosition(DeSerilizationVec3(json["Transform"]["position"]));
        Front = DeSerilizationVec3(json["lookat"]);
        fov_Y = json["fov"].number_value();
        aspect_ratio = json["aspect_ratio"].number_value();
        n = json["n"].number_value();
        f = json["f"].number_value();
        UpdateProjectionInfo();
    }
};
} // namespace ylb
#endif