#include "FrameBuffer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
using namespace ylb;

void FrameBuffer::clear()
{
    int length = h * w;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            unsigned char color = (y * w + x) * 255.f / length;
            set_depth(x, y, -1000);
            set_color(x, y, color, color, color);
        }
    }
}

void FrameBuffer::save_zbuffer(const char *fileName, bool perspective) {
    unsigned char *pixel = new unsigned char[w*h*3];

    for(int y = 0 ; y < h ; y++)
    {
        for(int x = 0 ; x < w ; x++)
        {
            int i = y*w+x;
            unsigned char color = 0;
            if(depth[i] > -1000)
            {
                color = perspective ? (depth[i] / 100 + 1) * 255 : (depth[i]+1) * 127;
            }
            int pi = y*w*3 + x *3;
            pixel[pi] = color;pixel[pi+1]=color;pixel[pi+2]=color;
        }
    }
    stbi_flip_vertically_on_write(1);
    stbi_write_bmp(fileName,w,h,3,pixel);
}

void FrameBuffer::save_frame(const char *fileName) {
    stbi_flip_vertically_on_write(1);
    stbi_write_bmp(fileName,w,h,3,pixels);
}
