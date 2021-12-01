#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H
#include "Math/Matrix.h"
#include "Math/Common.h"
namespace YYLB{
    struct FrameBuffer
    {
        float *depth;
        unsigned char *pixels;
        int w, h;
        FrameBuffer(size_t _w, size_t _h) : w(_w), h(_h)
        {
            size_t length = _h * _w;
            pixels = new unsigned char[length * 3];
            depth = new float[length];
        }

        inline void set_color(int &x, int &y, unsigned char &r, unsigned char &g, unsigned char &b)
        {
            int pixel = w * y * 3 + x * 3;
            pixels[pixel] = r;
            pixels[pixel + 1] = g;
            pixels[pixel + 2] = b;
        }

        inline void set_color(int &x, int &y, Vec3f &color)
        {
            YYLB::clamp(color,0.f,1.0f);
            int pixel = w * y * 3 + x * 3;
            pixels[pixel] = color.x() * 255.f;
            pixels[pixel + 1] = color.y() * 255.f;
            pixels[pixel + 2] = color.z() * 255.f;
        }

        inline void set_depth(int &x, int &y, const float &d)
        {
            depth[y * w + x] = d;
        }

        void clear();
    };
}
#endif