#ifndef YBT_MATH_TRIANGLE
#define YBT_MATH_TRIANGLE
#include "Matrix.h"
#include "Common.h"
#include "Core/Scene/Actor.h"
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

        Vec3f cof;
        YYLB::Vertex vts[3];
        Triangle() = default;
        
        Triangle(Vertex &vt1, Vertex &vt2, Vertex &vt3);
        Triangle(Vertex &&vt1, Vertex &&vt2, Vertex &&vt3) : vts{vt1,vt2,vt3}{}
        inline const BoundingBox *bounding_box()
        {
            return &bb;
        }
        void ready_to_raser(Vec4f pos_screen_space[]);
        float interpolated_depth();
        void interpolated_uv(float &u, float &v);
        void interpolated_color(Vec3f& color);
        Vec3f interpolated_world_position();
        Vec3f interpolated_world_normal();
        inline static bool inside(float x, float y, YYLB::Triangle &t)
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
            t.cof.x() = a;
            t.cof.y() = b;
            t.cof.z() = 1 - a - b;
            return a >= 0 && b >= 0 && a + b <= 1.0f;
        }
    };
}

#endif