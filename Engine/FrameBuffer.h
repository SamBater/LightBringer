#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glm/vec3.hpp>
#include "Common.h"
#include <algorithm>
#include "Texture.h"
#include "glm/fwd.hpp"
namespace ylb{
    struct FrameBuffer
    {
        Texture* depth;
        unsigned char *pixels;
        int w, h;
        FrameBuffer(size_t _w, size_t _h) : w(_w), h(_h)
        {
            size_t length = _h * _w;
            pixels = new unsigned char[length * 3];
            depth = new Texture(w,h);
        }

        inline void set_color(int x, int y, unsigned char r, unsigned char g, unsigned char b)
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
            depth->Fill(x,y,glm::vec3(d,d,d));
        }

        bool DepthTest(const int x,const int y,const float d){
            float u = x*1.0f / w;
            float v = y*1.0f / h;
            auto stored_depth = depth->tex2d(u,v).z;
            return d + ylb::eps < stored_depth;
        }

        void save_zbuffer(const char *fileName, bool perspective = true);

        void save_frame(const char*fileName);

        void clear();
    };
}
#endif