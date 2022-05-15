#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glm/vec3.hpp>
#include "Common.h"
namespace ylb{
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

        inline void set_color(int &x, int &y, glm::vec3 &color)
        {
            auto clamp = [](float& n)
            {
                if( n > 1 ) n = 1;
                if( n < 0 ) n = 0;
            };

            clamp(color.x);
            clamp(color.y);
            clamp(color.z);

            int pixel = w * y * 3 + x * 3;
            pixels[pixel]     = static_cast<unsigned char>(color.x * 255.f);
            pixels[pixel + 1] = static_cast<unsigned char>(color.y * 255.f);
            pixels[pixel + 2] = static_cast<unsigned char>(color.z * 255.f);
        }

        inline void set_depth(int &x, int &y, const float &d)
        {
            depth[y * w + x] = d;
        }

        void save_zbuffer(const char* fileName, bool perspective);

        void save_frame(const char*fileName);

        void clear();
    };
}
#endif