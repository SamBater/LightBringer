#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H
#include "Math/Matrix.h"
namespace YYLB{
    struct FrameBuffer
    {
        float *depth;
        unsigned char *pixels;
        int w, h;
        FrameBuffer(int _w, int _h) : w(_w), h(_h)
        {
            int length = h * w;
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

        inline void set_color(int &x, int &y, RGB &rgb)
        {
            int pixel = w * y * 3 + x * 3;
            pixels[pixel] = rgb.x();
            pixels[pixel + 1] = rgb.y();
            pixels[pixel + 2] = rgb.z();
        }

        inline void set_color(int &x, int &y, Vec3f &color)
        {
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