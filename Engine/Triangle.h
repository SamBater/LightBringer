#ifndef YBT_MATH_TRIANGLE
#define YBT_MATH_TRIANGLE
#include "glm/glm.hpp"
#include "Common.h"
#include "Actor.h"
#include "Vertex.h"
#include "YLBSerializable.h"
namespace ylb
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
        glm::vec3 cof;
        float s;
        ylb::Vertex vts[3];
        Triangle() = default;
        
        Triangle(Vertex &vt1, Vertex &vt2, Vertex &vt3);
        Triangle(Vertex &&vt1, Vertex &&vt2, Vertex &&vt3) : vts{vt1,vt2,vt3}{}
        inline const BoundingBox *bounding_box()
        {
            return &bb;
        }
        void ready_rasterization();
        float interpolated_depth();
        void interpolated_uv(float &u, float &v);
        glm::vec3 interpolated_world_position();
        glm::vec3 interpolated_world_normal();

        float interpolate_attribute1D(float attribute);
        glm::vec2 interpolate_attribute2D(const glm::vec2 attribute);
        glm::vec3 interpolate_attribute3D(const glm::vec3& vet);


        inline static bool inside(float x, float y, ylb::Triangle &t)
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
            t.cof.x = a;
            t.cof.y = b;
            t.cof.z = 1 - a - b;
            t.s = 1 / (t.cof.x * t.vts[0].inv + t.cof.y * t.vts[1].inv + t.cof.z * t.vts[2].inv);
            return a >= 0 && b >= 0 && a + b <= 1.0f;
        }

    };
    }

#endif