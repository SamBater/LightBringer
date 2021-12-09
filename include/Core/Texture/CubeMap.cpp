//
// Created by a on 2021/12/9.
//

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
        return faces->at(x > 0 ? 0 : 1)->tex2d(vec.y,vec.z);
    }

    if(max3(x,y,z) == y)
    {
        vec /= y;
        vec += 1;
        vec *= 0.5f;
        return faces->at(y > 0 ?  2 : 3)->tex2d(vec.x,vec.z);
    }

    if(max3(x,y,z) == z)
    {
        vec /= z;
        vec += 1;
        vec *= 0.5f;
        return faces->at(z > 0 ? 4 : 5)->tex2d(vec.x,vec.y);
    }
}
