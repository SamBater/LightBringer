#ifndef YYLB_CAMERA
#define YYLB_CAMERA
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "Actor.h"
#include "YLBSerializable.h"
namespace ylb {
class Transformer;

enum class PROJECTION_MODE {
    PERSPECTIVE,
    ORTHOGONAL
};

class Camera : public Actor, public YLBSerializable {
private:
    double fov_Y;
    double aspect_ratio;
    double l, r, t, b, n, f;
    friend class ylb::Transformer;
    friend class Renderer;

public:
    enum PROJECTION_MODE mode;
    glm::vec3 look_at; //看的朝向
    glm::vec3 up;      //向上方向
    Camera() = default;
    Camera(const glm::vec3 &origin, const glm::vec3 &lookat, float fov_v,
           float _aspect_ratio, float _n, float _f);
    void set_perspective_matrix(glm::mat4 &mat);
    void set_orthogonal_matrix(glm::mat4 &mat);
    virtual void DeSerilization(const json11::Json &json) override {
        position_world = DeSerilizationVec3(json["Transform"]["position"]);
        look_at = DeSerilizationVec3(json["lookat"]);
        fov_Y = json["fov"].number_value();
        aspect_ratio = json["aspect_ratio"].number_value();
        n = json["n"].number_value();
        f = json["f"].number_value();

        t = std::tan(fov_Y / 2) * n;
        r = aspect_ratio * t;
        l = -r;
        b = -t;
        up = {0, 1, 0};
    }
};
} // namespace ylb
#endif