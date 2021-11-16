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
        Vec2f v2d[3];
        Vec2f A, B, C;
        BoundingBox *bb;

    public:
        Triangle() = default;
        Vec3f v3d_local[3];
        Triangle(Vec3f &a, Vec3f &b, Vec3f &c) : v3d_local{a, b, c}
        {

            //TODO:替换真正的屏幕坐标 暂且如此设置
            // for (int r = 0; r < 3; r++)
            // {
            //     v2d[r].x() = v3d_local[r].x();
            //     v2d[r].y() = v3d_local[r].y();
            // }

            // A = new Vec2f{v2d[1] - v2d[0]};
            // B = new Vec2f{v2d[2] - v2d[1]};
            // C = new Vec2f{v2d[0] - v2d[2]};

            bb = new BoundingBox();
        }

        //通过MVP,准备进行光栅化
        void ready_to_raser(Vec4f pos_screen_space[])
        {
            for (int r = 0; r < 3; r++)
            {
                v2d[r].x() = pos_screen_space[r].x();
                v2d[r].y() = pos_screen_space[r].y();
            }

            A = v2d[1] - v2d[0];
            B = v2d[2] - v2d[1];
            C = v2d[0] - v2d[2];
            bb->top = YYLB::max3(v2d[0].y(), v2d[1].y(), v2d[2].y());
            bb->bot = YYLB::min3(v2d[0].y(), v2d[1].y(), v2d[2].y());
            bb->right = YYLB::max3(v2d[0].x(), v2d[1].x(), v2d[2].x());
            bb->left = YYLB::min3(v2d[0].x(), v2d[1].x(), v2d[2].x());
            //printf("%f %f %f %f\n", bb->top, bb->bot, bb->left, bb->right);
        }

        const BoundingBox *bounding_box()
        {
            return bb;
        }

        inline static bool inside(int x, int y, YYLB::Triangle &t)
        {
            auto p = YYLB::Vec2f{x + 0.5f, y + 0.5f};

            auto PA = p - t.v2d[0];
            auto PB = p - t.v2d[1];
            auto PC = p - t.v2d[2];

            auto ra = cross_product(PA, (t.A));
            auto rb = cross_product(PB, (t.B));
            auto rc = cross_product(PC, (t.C));
            bool ok = ra >= 0 && rb >= 0 && rc >= 0 || ra <= 0 && rb <= 0 && rc <= 0;
            return ok;
        }
    };
}

#endif