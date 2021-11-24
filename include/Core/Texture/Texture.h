#pragma once
#include "Math/Matrix.h"
#include "Math/Common.h"
namespace YYLB
{
    class Texture
    {
    protected:
        unsigned char *sprite;
        int w;
        int h;
        int comp;

    public:
        Texture() = default;
        Texture(const char *file_name);
        virtual Vec3f tex2d(const float &tex_cord_x, const float &tex_cord_y);
        virtual ~Texture();
    };
}