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
        Texture* depth_buffer;
        unsigned char *color_buffer;
        int w, h;
        FrameBuffer(size_t _w, size_t _h) : w(_w), h(_h)
        {
            size_t length = _h * _w;
            color_buffer = new unsigned char[length * 3];
            depth_buffer = new Texture(w,h);
        }

        inline void set_color(int x, int y, unsigned char r, unsigned char g, unsigned char b)
        {
            int pixel = w * y * 3 + x * 3;
            color_buffer[pixel] = r;
            color_buffer[pixel + 1] = g;
            color_buffer[pixel + 2] = b;
        }

        inline void set_color(int x, int y, glm::vec3 &color)
        {
            ylb::clamp(color,0.0f,1.0f);
            int pixel = w * y * 3 + x * 3;
            color_buffer[pixel]     = static_cast<unsigned char>(color.x * 255.f);
            color_buffer[pixel + 1] = static_cast<unsigned char>(color.y * 255.f);
            color_buffer[pixel + 2] = static_cast<unsigned char>(color.z * 255.f);
        }

        inline void SetDepth(int x, int y, const float depth)
        {
            depth_buffer->Fill(x,y,glm::vec3(depth,depth,depth));
        }

        bool DepthTest(const int x,const int y,const float depth){
            float u = x*1.0f / w;
            float v = y*1.0f / h;
            auto stored_depth = depth_buffer->tex2d(u,v).z;
            return depth + ylb::eps <= stored_depth;
        }

        void SaveDepthMap(const char *fileName, bool perspective = true);

        void SaveFrame(const char*fileName);

        void Clear();
    };
}
#endif