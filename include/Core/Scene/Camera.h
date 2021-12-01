#ifndef YYLB_CAMERA
#define YYLB_CAMERA
#include "Math/Matrix.h"
#include "Core/Scene/Actor.h"
namespace YYLB
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
        friend class YYLB::Transformer;
        float size = 20.f;

    public:
        enum PROJECTION_MODE mode;
        Vec3f look_at; //看的朝向
        Vec3f up;      //向上方向
        Camera(const float x, const float y,
               const float z, float fov_v,
               float _aspect_ratio, float _n, float _f);
        void set_perspective_matrix(Matrix4f &mat);
        void set_orthogonal_matrix(Matrix4f& mat);
    };
}
#endif