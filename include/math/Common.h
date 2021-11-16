#ifndef YBT_MATH_COMMON
#define YBT_MATH_COMMON
namespace YYLB
{
    const float PI = 3.1415926f;
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

    inline float theta_to_degree(float theta)
    {
        return theta / 180 * PI;
    }
}
#endif