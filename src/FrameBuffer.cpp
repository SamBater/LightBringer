#include "Core/Pipeline/FrameBuffer.h"
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