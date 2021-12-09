//
// Created by a on 2021/12/9.
//

#ifndef LIGHTBRINGER_CUBEMAP_H
#define LIGHTBRINGER_CUBEMAP_H
#include <vector>
#include <string>
#include <iostream>
#include "Texture.h"
#include "Image/stb_image.h"

namespace ylb
{
    class CubeMap : public Texture {
    private:
        std::vector<Texture*>* faces;
        virtual glm::vec3 tex2d(const float &tex_cord_x, const float &tex_cord_y) override;
    public:
        CubeMap(std::vector<std::string> &faces_path);
        virtual glm::vec3 tex3d(glm::vec3& vec);
    };
}


#endif //LIGHTBRINGER_CUBEMAP_H
