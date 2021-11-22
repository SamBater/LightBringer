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
    }
}