#ifndef YYLB_MESH
#define YYLB_MESH
#include <math/Matrix.h>
#include <math/Triangle.h>
#include <Core/Actor.h>
#include <vector>
namespace YYLB
{
    class Mesh : public Actor
    {
    private:
        std::vector<Triangle> triangles;

    public:
        Mesh(const float x, const float y, const float z, std::vector<Triangle> &&ts) : Actor(x, y, z), triangles(ts) {}
        inline const std::vector<Triangle> &get_triangles() const { return triangles; }
        inline std::vector<Triangle> &get_triangles() { return triangles; }
    };
}
#endif