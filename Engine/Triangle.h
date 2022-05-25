#ifndef YBT_MATH_TRIANGLE
#define YBT_MATH_TRIANGLE
#include "Actor.h"
#include "Common.h"
#include "Vertex.h"
#include "YLBSerializable.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

namespace ylb {
struct BoundingBox {
    float top, bot, left, right;
};

class Triangle : public Actor {
private:
    BoundingBox bb;
    glm::vec3 ComputeBarycentric(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P);

public:
    glm::vec3 bar;
    float inv_w;
    ylb::Vertex vts[3];
    Triangle() = default;

    Triangle(const Vertex &vt1, const Vertex &vt2, const Vertex &vt3) :
        vts{vt1, vt2, vt3} {
    }
    inline const BoundingBox *bounding_box() {
        return &bb;
    }
    void ComputeBoundingBox();
    float interpolated_depth();
    void interpolated_uv(float &u, float &v);
    glm::vec3 interpolated_world_position();
    glm::vec3 interpolated_world_normal();

    inline static bool Inside(float x, float y, ylb::Triangle &t, bool perspective = true) {
        t.bar = t.ComputeBarycentric(t.vts[0].screen_coords, t.vts[1].screen_coords, t.vts[2].screen_coords, glm::vec3(x, y, 0.0));
        t.inv_w = 0;
        for (int i = 0; i < 3; i++)
            t.inv_w += t.vts[i].screen_coords[3] * t.bar[i];
        t.inv_w = perspective ? 1.0 / t.inv_w : 1.0;
        return t.bar.x >= 0 && t.bar.y >= 0 && t.bar.z >= 0.0f;
    }
};
} // namespace ylb

#endif