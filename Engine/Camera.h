#ifndef YYLB_CAMERA
#define YYLB_CAMERA
#include "glm/glm.hpp"
#include "Actor.h"
namespace ylb
{
    class Transformer;

    enum class PROJECTION_MODE {
        PERSPECTIVE,
        ORTHOGONAL
    };

    class Camera : public Actor
    {
    private:
        float fov_Y;
        float aspect_ratio;
        float l, r, t, b, n, f;
        friend class ylb::Transformer;
        friend class Renderer;

    public:
        enum PROJECTION_MODE mode;
        glm::vec3 look_at; //看的朝向
        glm::vec3 up;      //向上方向
        Camera(const float x, const float y,
               const float z, float fov_v,
               float _aspect_ratio, float _n, float _f);
        void set_perspective_matrix(glm::mat4 &mat);
        void set_orthogonal_matrix(glm::mat4 &mat);
    };
}
#endif