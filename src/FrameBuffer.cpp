#include "Core/Pipeline/FrameBuffer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image/stb_image_write.h"
using namespace YYLB;

void FrameBuffer::clear()
{
    int length = h * w;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
             unsigned char color = (y * w + x) * 255.f / length;
            //unsigned char color = 0;
            set_depth(x, y, -1000);
            set_color(x, y, color, color, color);
        }
    }
}

void FrameBuffer::save(const char *fileName,bool perspective) {
    unsigned char *pixel = new unsigned char[w*h*3];
    float minz=1000,maxz=-1002;
    for(int y = 0 ; y < h ; y++)
    {
        for(int x = 0 ; x < w ; x++)
        {
            int i = y*w+x;
            unsigned char color = 0;
            if(depth[i] > -1000)
            {
                color = perspective ? (1 - depth[i])*255 : (depth[i] + 1) * 127;
                minz = std::min(minz,depth[i]);
                maxz = std::max(maxz,depth[i]);
            }
            int pi = y*w*3 + x *3;
            pixel[pi] = color;pixel[pi+1]=color;pixel[pi+2]=color;
        }
    }
    printf("%f %f\n",minz,maxz);
    stbi_flip_vertically_on_write(1);
    stbi_write_bmp(fileName,w,h,3,pixel);
}
