#include "Core/Scene/Camera.h"
namespace YYLB
{
    Camera::Camera(const float x, const float y, const float z,
                   float fov_v, float _aspect_ratio,
                   float _n, float _f) : Actor(x, y, z), fov_Y(fov_v),
                                         aspect_ratio(_aspect_ratio), n(_n), f(_f)
    {
        t = std::tan(fov_Y / 2) * n;
        r = aspect_ratio * t;
        l = -r;
        b = -t;

        look_at = {0.5,1,-2};
        look_at.normalized();
        up = {0,1,0};
    }

    void Camera::set_perspective_matrix(Matrix4f &mat) {
        set_zero(mat);
        mat[0][0] = 2 * n / (r - l);
        mat[0][2] = (l + r) / (l - r);
        mat[1][1] = 2 * n / (t - b);
        mat[1][2] = (b + t) / (b - t);
        mat[2][2] = (f + n) / (n - f);
        mat[2][3] = 2 * f * n / (f - n);
        mat[3][2] = 1;
    }

    void Camera::set_orthogonal_matrix(Matrix4f &mat) {
        set_identyti(mat);
        mat[0][0] = 2 / (r-l);
        mat[1][1] = 2 / (t-b);
        mat[2][2] = 2 / (n-f);
        mat[0][3] = -(r+l) / (r-l);
        mat[1][3] = -(t+b) / (t-b);
        mat[2][3] = -(n+f) / (n-f);
    }
}