#include <math/Triangle.h>

using namespace YYLB;

Triangle::Triangle(Vertex &vt1, Vertex &vt2, Vertex &vt3) : vts{vt1, vt2, vt3}
{
}

void Triangle::ready_to_raser(Vec4f pos_screen_space[])
{
    for (int r = 0; r < 3; r++)
    {
        vts[r].sx() = pos_screen_space[r].x();
        vts[r].sy() = pos_screen_space[r].y();
        vts[r].sz() = pos_screen_space[r].z();
    }

    float i = 1.f;
    area = det(vts[0].sx(), vts[1].sx(), vts[2].sx(),
               vts[0].sy(), vts[1].sy(), vts[2].sy(),
               i, i, i);
    bb.top = YYLB::max3(vts[0].sy(), vts[1].sy(), vts[2].sy());
    bb.bot = YYLB::min3(vts[0].sy(), vts[1].sy(), vts[2].sy());
    bb.right = YYLB::max3(vts[0].sx(), vts[1].sx(), vts[2].sx());
    bb.left = YYLB::min3(vts[0].sx(), vts[1].sx(), vts[2].sx());
}

float Triangle::interpolated_depth(Vec3f &cof)
{
    float s = 1 / (cof.x() * vts[0].inv + cof.y() * vts[1].inv + cof.z() * vts[2].inv);
    return (cof.x() * vts[0].sz() +
            cof.y() * vts[1].sz() +
            cof.z() * vts[2].sz()) *
           s;
}

void Triangle::interpolated_uv(Vec3f &cof, float &u, float &v)
{
    float s = 1 / (cof.x() * vts[0].inv + cof.y() * vts[1].inv + cof.z() * vts[2].inv);
    u = s * (cof.x() * vts[0].u() + cof.y() * vts[1].u() + cof.z() * vts[2].u());
    v = s * (cof.x() * vts[0].v() + cof.y() * vts[1].v() + cof.z() * vts[2].v());
}