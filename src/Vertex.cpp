#include <math/Vertex.h>

using namespace YYLB;

Vertex::Vertex(Vec3f &&position, Vec2f &&uv) : position(position), uv(uv)
{
}

Vertex::Vertex(Vec3f &position, Vec2f &uv) : position(position), uv(uv)
{
}