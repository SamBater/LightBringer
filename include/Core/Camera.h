#ifndef YYLB_CAMERA
#define YYLB_CAMERA
#include <math/Matrix.h>
#include <Core/Actor.h>
namespace YYLB
{
    class Transformer;
    class Camera : public Actor
    {
    private:
        float fov_Y;
        float aspect_ratio;
        float l, r, t, b, n, f;
        friend class YYLB::Transformer;
        Vec3f look_at; //看的朝向
        Vec3f up;      //向上方向

    public:
        Camera(const float x, const float y,
               const float z, float fov_v,
               float _aspect_ratio, float _n, float _f);
        void set_perspective_matrix(Matrix4f &mat)
        {
            set_zero(mat);
            mat[0][0] = 2 * n / (r - l);
            mat[0][2] = (l + r) / (l - r);
            mat[1][1] = 2 * n / (t - b);
            mat[1][2] = (b + t) / (b - t);
            mat[2][2] = (f + n) / (n - f);
            mat[2][3] = 2 * f * n / (f - n);
            mat[3][2] = 1;

            // set_identyti(mat);
            // mat[0][0] = 2 / (r - l);
            // mat[1][1] = 2 / (t - b);
            // mat[2][2] = 2 / (n - f);

            // Matrix4f offset;
            // set_identyti(offset);
            // offset[0][3] = -(r + l) / 2;
            // offset[1][3] = -(t + b) / 2;
            // offset[2][3] = -(f + n) / 2;

            // Matrix4f pers;
            // set_zero(pers);
            // pers[0][0] = pers[1][1] = n;
            // pers[2][2] = n + f;
            // pers[2][3] = -n * f;
            // pers[3][2] = 1;

            // mat *= offset;
            // mat *= pers;
        }
    };
}
#endif