#ifndef YYLB_MESH
#define YYLB_MESH
#include "Matrix.h"
#include "Core/Scene/Actor.h"
#include "Triangle.h"
#include "Core/Shader/Shader.h"
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
        Mesh(const float x, const float y, const float z, std::vector<Triangle>& ts) : Actor(x, y, z), triangles(ts) {}
        inline const std::vector<Triangle> &get_triangles() const { return triangles; }
        inline std::vector<Triangle> &get_triangles() { return triangles; }
        void rotate(Vec3f axis,float theta)
        {
            auto rot = YYLB::rotate_transform(axis,theta);
            for(auto &t : triangles)
            {
                for(Vertex& v : t.vts)
                {
                    auto pos = rot * v.position;
                    auto normal = rot * v.normal;
                    v.set_pos(pos.x(),pos.y(), pos.z());
                    v.set_normal(normal);
                }
            }
        }

        void scale_transform(float sx,float sy,float sz)
        {
            for(auto &t : triangles)
            {
                for(auto& v : t.vts)
                {
                    v.position.x() = v.position.x() * sx;
                    v.position.y() = v.position.y() * sy;
                    v.position.z() = v.position.z() * sz;
                }
            }
        }
    };
}
#endif