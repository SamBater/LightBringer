#ifndef YBT_MATH_TRIANGLE
#define YBT_MATH_TRIANGLE
#include <math/Matrix.h>
#include <math/Common.h>
#include <Core/Actor.h>
#include <math/Vertex.h>
namespace YYLB
{
    struct BoundingBox
    {
        float top, bot, left, right;
    };

    class Triangle : public Actor
    {

    private:
        BoundingBox bb;
        float area;

    public:
        YYLB::Vertex vts[3];
        Triangle() = default;
        Triangle(Vertex &vt1, Vertex &vt2, Vertex &vt3);
        inline const BoundingBox *bounding_box()
        {
            return &bb;
        }
        void ready_to_raser(Vec4f pos_screen_space[]);
        float interapoted_depth(Vec3f &cof);
        void interapoted_uv(Vec3f &cof, float &u_out, float &v_out);
        inline static bool inside(float x, float y, YYLB::Triangle &t, Vec3f &interapted_cof)
        {
            float i = 1.0f;
            float a = det(x, t.vts[1].sx(), t.vts[2].sx(),
                          y, t.vts[1].sy(), t.vts[2].sy(),
                          i, i, i) /
                      t.area;

            float b = det(t.vts[0].sx(), x, t.vts[2].sx(),
                          t.vts[0].sy(), y, t.vts[2].sy(),
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