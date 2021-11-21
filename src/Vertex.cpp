#include <math/Vertex.h>

using namespace YYLB;

Vertex::Vertex(Vec3f &&position, Vec3f &&normal,Vec2f &&uv) : position(position), normal(normal),uv(uv)
{
}

Vertex::Vertex(Vec3f &position, Vec3f& normal, Vec2f &uv) : position(position), normal(normal),uv(uv)
{
}