#ifndef YBT_MATH_COMMON
#define YBT_MATH_COMMON
#include "glm/glm.hpp"
namespace ylb
{
    const float PI = 3.1415926f;
    const float eps = 1e-6f;
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
    inline T clamp(const T& val, const T&& min, const T&& max)
    {
        T rVal = val;
        if (rVal > max)
            rVal = max;
        if (rVal < min)
            rVal = min;
        return rVal;
    }

    inline glm::vec3 &clamp(glm::vec3& val, const float min, const float max)
    {
        if(val.x < min) val.x = min;
        if(val.y < min) val.y = min;
        if(val.z < min) val.z = min;
        if(val.x > max) val.x = max;
        if(val.y > max) val.y = max;
        if(val.z > max) val.z = max;
        return val;
    }

    template<typename T>
    T lerp(T& x, T& y,float t)
    {
        return x + (y-x) * t;
    }

    inline float theta_to_degree(float theta)
    {
        return theta / 180 * PI;
    }

}
#endif