#ifndef YYLB_TEXTURE_H
#define YYLB_TEXTURE_H
#include "Math/Matrix.h"

namespace YYLB
{
    class Texture
    {
    private:
        unsigned char *texture;
        int w;
        int h;
        int comp;

    public:
        Texture();
        Texture(const char *file_name);
        void tex2d(const float &tex_cord_x, const float &tex_cord_y, RGB &color);
        virtual ~Texture();
    };
}

#endif