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

class Camera : public Actor, public YLBSerializable {
public:

    Camera() = default;


    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetPerspectiveMatrix();
    glm::mat4 GetOrthogonalMatrix();
 
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void UpdateCameraInfo();

    virtual void DeSerilization(const json11::Json &json) override {
        transform.SetPosition(DeSerilizationVec3(json["Transform"]["position"]));
        Front = DeSerilizationVec3(json["lookat"]);
        fov_Y = json["fov"].number_value();
        aspect_ratio = json["aspect_ratio"].number_value();
        n = json["n"].number_value();
        f = json["f"].number_value();
        UpdateCameraInfo();
    }

private:
    float fov_Y;
    float aspect_ratio;
    float n, f, l, r, b, t;
    enum PROJECTION_MODE mode;
    // camera Attributes
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);
    // euler Angles
    float Yaw = -90;
    float Pitch = 0;
    // camera options
    float MovementSpeed = 5.0f;
    float MouseSensitivity = 0.1f;
    float Zoom = 45.0f;
    friend class Renderer;
};
} // namespace ylb
#endif