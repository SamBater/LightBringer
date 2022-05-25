#include "CubeMap.h"


using namespace ylb;

CubeMap::CubeMap(std::vector<std::string> &faces_path) {
    faces = new std::vector<Texture*>();
    for (unsigned int i = 0; i < faces_path.size(); i++)
    {
        faces->push_back(new Texture(faces_path[i].c_str()));
    }
}

glm::vec3 CubeMap::tex2d(const float &tex_cord_x, const float &tex_cord_y) {
    return Texture::tex2d(tex_cord_x, tex_cord_y);
}

glm::vec3 CubeMap::tex3d(glm::vec3 &vec) {
    int x = abs(vec.x);
    int y = abs(vec.y);
    int z = abs(vec.z);
    if(max3(x,y,z) == x)
    {
        vec /= x;
        vec += 1;
        vec *= 0.5f;
        if(vec.x > 0)
            return faces->at( 0 )->tex2d(vec.y,vec.z);
        return faces->at( 1 )->tex2d(vec.y,vec.z);
    }

    else if(max3(x,y,z) == y)
    {
        vec /= y;
        vec += 1;
        vec *= 0.5f;
        if(y > 0)
            return faces->at(2)->tex2d(vec.x,vec.z);
        return faces->at(3)->tex2d(vec.x,vec.z);
    }

    else
    {
        vec /= z;
        vec += 1;
        vec *= 0.5f;
        if(z > 0 )
            return faces->at(4)->tex2d(vec.x,vec.y);
        return faces->at( 5)->tex2d(vec.x,vec.y);
    }
}
