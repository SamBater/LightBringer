#ifndef YBT_MATH_TRIANGLE
#define YBT_MATH_TRIANGLE
#include <math/Matrix.h>
#include <math/Common.h>
#include <Core/Actor.h>
namespace YYLB
{
    struct BoundingBox
    {
        float top, bot, left, right;
    };
    class Triangle : public Actor
    {

    private:
        //局部坐标系

        //屏幕坐标系

        Vec3f A, B, C;
        BoundingBox *bb;
        float area;

    public:
        Triangle() = default;
        unsigned char debugColor[3];
        Vec3f v2d[3];
        Vec3f v3d_local[3];
        Triangle(Vec3f a, Vec3f b, Vec3f c, std::initializer_list<unsigned char> colors) : v3d_local{a, b, c}
        {
            int i = 0;
            for (auto c = colors.begin(); c != colors.end(); c++)
                debugColor[i++] = *c;
            bb = new BoundingBox();
        }

        //通过MVP,准备进行光栅化
        void ready_to_raser(Vec4f pos_screen_space[])
        {

            //TODO 深度插值
            for (int r = 0; r < 3; r++)
            {
                v2d[r].x() = pos_screen_space[r].x();
                v2d[r].y() = pos_screen_space[r].y();
                v2d[r].z() = pos_screen_space[r].z();
            }

            float i = 1.f;
            area = det(v2d[0].x(), v2d[1].x(), v2d[2].x(),
                       v2d[0].y(), v2d[1].y(), v2d[2].y(),
                       i, i, i);
            // A = v2d[1] - v2d[0];
            // B = v2d[2] - v2d[1];
            // C = v2d[0] - v2d[2];

            bb->top = YYLB::max3(v2d[0].y(), v2d[1].y(), v2d[2].y());
            bb->bot = YYLB::min3(v2d[0].y(), v2d[1].y(), v2d[2].y());
            bb->right = YYLB::max3(v2d[0].x(), v2d[1].x(), v2d[2].x());
            bb->left = YYLB::min3(v2d[0].x(), v2d[1].x(), v2d[2].x());
        }

        const BoundingBox *bounding_box()
        {
            return bb;
        }

        inline static bool inside(float x, float y, YYLB::Triangle &t, Vec3f &interapted_cof)
        {

            //叉乘法：
            // auto p = YYLB::Vec2f{x, y};

            // auto PA = p - t.v2d[0];
            // auto PB = p - t.v2d[1];
            // auto PC = p - t.v2d[2];

            // auto ra = cross_product(PA, (t.A));
            // auto rb = cross_product(PB, (t.B));
            // auto rc = cross_product(PC, (t.C));
            // bool ok = ra >= 0 && rb >= 0 && rc >= 0 || ra <= 0 && rb <= 0 && rc <= 0;
            // return ok;

            //重心坐标法
            float i = 1.0f;
            float a = det(x, t.v2d[1].x(), t.v2d[2].x(),
                          y, t.v2d[1].y(), t.v2d[2].y(),
                          i, i, i) /
                      t.area;

            float b = det(t.v2d[0].x(), x, t.v2d[2].x(),
                          t.v2d[0].y(), y, t.v2d[2].y(),
                          i, i, i) /
                      t.area;
            interapted_cof.x() = a;
            interapted_cof.y() = b;
            interapted_cof.z() = 1 - a - b;
            return a >= 0 && b >= 0 && a + b <= 1.0f;
        }
    };
}

#endif