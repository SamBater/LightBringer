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
  float area;
  glm::vec3 barycentric(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P) {
    glm::vec3 s[2];
    //计算[AB,AC,PA]的x和y分量
    for (int i = 2; i--;) {
      s[i][0] = C[i] - A[i];
      s[i][1] = B[i] - A[i];
      s[i][2] = A[i] - P[i];
    }
    //[u,v,1]和[AB,AC,PA]对应的x和y向量都垂直，所以叉乘
    auto u = glm::cross(s[0], s[1]);
    //三点共线时，会导致u[2]为0，此时返回(-1,1,1)
    if (std::abs(u[2]) > 1e-2)
      //若1-u-v，u，v全为大于0的数，表示点在三角形内部
      return glm::vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return glm::vec3(-1, 1, 1);
  }

public:
  glm::vec3 cof;
  float s;
  float ls;
  ylb::Vertex vts[3];
  Triangle() = default;

  Triangle(const Vertex &vt1, const Vertex &vt2, const Vertex &vt3)
      : vts{vt1, vt2, vt3} {
      }
  inline const BoundingBox *bounding_box() { return &bb; }
  void ready_rasterization();
  float interpolated_depth();
  void interpolated_uv(float &u, float &v);
  glm::vec3 interpolated_world_position();
  glm::vec3 interpolated_world_normal();

  inline static bool inside(float x, float y, ylb::Triangle &t , bool perspective = true) {
    t.cof = t.barycentric(t.vts[0].sv_pos, t.vts[1].sv_pos, t.vts[2].sv_pos, glm::vec3(x,y,0.0));
    t.s = 0;
    for(int i = 0 ; i < 3 ; i++)
        t.s += t.vts[i].inv * t.cof[i];
    t.s =  perspective ? 1.0 / t.s : 1.0;

    if(perspective){
          t.ls = 0;
    for(int i = 0 ; i < 3 ; i++)
      t.ls += t.vts[i].light_coord.w * t.cof[i];
    t.ls = 1.0 / t.ls;
    }
    return t.cof.x >= 0 && t.cof.y >= 0 && t.cof.z >= 0.0f;
  }

  
};
} // namespace ylb

#endif