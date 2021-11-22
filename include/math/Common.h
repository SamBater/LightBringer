#ifndef YBT_MATH_COMMON
#define YBT_MATH_COMMON
#include "Matrix.h"
namespace YYLB
{
    const float PI = 3.1415926f;
    const float eps = 1e-6;
    template <typename T>
    inline T min(const T &a, const T &b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    inline T max(const T &a, const T &b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    inline T max3(const T &a, const T &b, const T &c)
    {
        return max(max(a, b), c);
    }

    template <typename T>
    inline T min3(const T &a, const T &b, const T &c)
    {
        return min(min(a, b), c);
    }

    template <typename T>
    T &clamp(T val, const T min, const T max)
    {
        if (val > max)
            val = max;
        if (val < min)
            val = min;
        return val;
    }

    inline float theta_to_degree(float theta)
    {
        return theta / 180 * PI;
    }

    inline float det(Matrix4f &mat)
    {
        return 0;
    }

    inline float det(float &a11, float &a12, float &a13,
                     float &a21, float &a22, float &a23,
                     float &a31, float &a32, float &a33)
    {
        return a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31 - a12 * a21 * a33 - a11 * a23 * a32;
    }

    inline float det(float &&a11, float &&a12, float &&a13,
                     float &&a21, float &&a22, float &&a23,
                     float &&a31, float &&a32, float &&a33)
    {
        return a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31 - a12 * a21 * a33 - a11 * a23 * a32;
    }
}
#endif