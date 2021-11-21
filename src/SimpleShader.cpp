#include "Core/SimpleShader.h"
#include "Core/ParallelLight.h"
#include "math/Matrix.h"

using namespace YYLB;

YYLB::Vec3f SimpleShader::shading(Vertex& v,Light* l)
{
    return Vec3f{1,1,1};
}

Vec3f SimpleShader::fragment_shading(Triangle& t,Light* l)
{
    RGB color;    
    float u,v;
    auto position_world = t.interpolated_world_position();
    auto normal = t.interpolated_world_normal();
    Vec3f l_dir;

    auto sun = static_cast<ParalleLight*>(l);
    if(sun!= nullptr)
    {
        l_dir = sun->dir * -1.0f;
    }
    else
    {
        l_dir = l->getPos() - position_world;
    }
    
    l_dir.normalized();
    float nxl= dot_product(l_dir,normal);

    l->light_intense = 3.f;
    l->light_color = Vec3f{1,1,1};

    float distance = l_dir.scalar();
    //float abt = 1 / (distance * distance);
    float abt = 1.0f;
    t.interpolated_uv(u,v);
    texture->tex2d(u,v,color);
    Vec3f tcolor{color.x()/255.f,color.y()/255.f,color.z()/255.f};

    
   
    Vec3f diffuse = l->light_color * std::max(0.f,nxl) * l->light_intense * abt  ;
    // Vec3f diffuse = l->light_color * (nxl + 1.f) * 0.5f * l->light_intense * abt;
    Vec3f ambient = {0.f,0.f,0.f};
    return diffuse + ambient ;
}
