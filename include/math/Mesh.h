#ifndef YYLB_MESH
#define YYLB_MESH
#include "math/Matrix.h"
#include <math/Vertex.h>
#include <math/Matrix.h>
#include <math/Triangle.h>
#include <Core/Actor.h>
#include <vector>
#include "Core/SimpleShader.h"
namespace YYLB
{
    class Render;
    class Mesh : public Actor
    {
    private:

        
    public:
        std::vector<Triangle> triangles;
        YYLB::Shader* shader;
        Mesh(const float x, const float y, const float z, std::vector<Triangle> &&ts) : Actor(x, y, z), triangles(ts) {}
        inline const std::vector<Triangle> &get_triangles() const { return triangles; }
        inline std::vector<Triangle> &get_triangles() { return triangles; }
        void rotate(float theta)
        {
            auto rot = YYLB::rotation_y_matrix4f(theta);
            for(auto &t : triangles)
            {
                for(Vertex& v : t.vts)
                {
                    Vec4f pos_h = {v.x(),v.y(),v.z(),1}; 
                    Vec4f normal_h = {v.normal.x(),v.normal.y(),v.normal.z(),0};
                    auto pos = rot * pos_h; 
                    auto n = rot * normal_h;
                    auto normal = Vec3f{n.x(),n.y(),n.z()};
                    v.set_pos(pos.x(),pos.y(), pos.z());
                    v.set_normal(normal);
                }
            }
        }
    };
}
#endif