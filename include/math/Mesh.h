#ifndef YYLB_MESH
#define YYLB_MESH
#include <math/Matrix.h>
#include <math/Triangle.h>
#include <vector>
namespace YYLB
{
    class Mesh
    {
    private:
        //世界坐标
        Vec3f worldPos;
        std::vector<Triangle> triangles;

    public:
        Mesh(const float x, const float y, const float z) : worldPos{x, y, z} {}
        inline const std::vector<Triangle> &get_triangles() const { return triangles; }
        inline std::vector<Triangle> &get_triangles() { return triangles; }
        inline Vec3f &get_worldPos() { return worldPos; }
        inline const Vec3f &get_worldPos() const { return worldPos; }
    };
}
#endif