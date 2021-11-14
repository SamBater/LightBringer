#ifndef YBT_MATH_TRIANGLE
#define YBT_MATH_TRIANGLE
#include <math/Matrix.h>
namespace YYLB
{

    class Triangle
    {
    private:
        Vec2i points[3];
        Vec2i A, B, C;

    public:
        Triangle() = default;
        Triangle(Vec2i &a, Vec2i &b, Vec2i &c) : points{a, b, c}
        {
            A = points[1] - points[0];
            B = points[2] - points[1];
            C = points[0] - points[2];
        }
        inline Vec2i operator[](int i) const { return points[i]; }
        inline Vec2i &operator[](int i) { return points[i]; }
        inline static bool inside(int x, int y, YYLB::Triangle &t)
        {
            auto p = YYLB::Vec2i{x, y};

            auto PA = p - t[0];
            auto PB = p - t[1];
            auto PC = p - t[2];

            auto ra = cross_product(PA, t.A);
            auto rb = cross_product(PB, t.B);
            auto rc = cross_product(PC, t.C);
            return ra >= 0 && rb >= 0 && rc >= 0;
        }
    };
}

#endif