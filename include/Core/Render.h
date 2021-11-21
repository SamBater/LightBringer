#ifndef YYLB_RENDER_H
#define YYLB_RENDER_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GLFW/glfw3.h>
#include <math/Triangle.h>
#include <math/Common.h>
#include <math/Matrix.h>
#include <Core/Camera.h>
#include <Core/Actor.h>
#include <math/Mesh.h>
#include <vector>
#include <chrono>
#include <Core/Transformer.h>
#include <Core/Texture.h>
#include <math/Vertex.h>
#include "Core/ParallelLight.h"
#include "Core/SimpleShader.h"
namespace YYLB
{
    class Render
    {
    private:
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

            inline void set_color(int &x, int &y, Vec3f& color)
            {
                int pixel = w * y * 3 + x * 3;
                if(color.x() > 1.0f) color.x() = 1.0f;
                if(color.y() > 1.0f) color.y() = 1.0f;
                if(color.z() > 1.0f) color.z() = 1.0f;
                pixels[pixel] = color.x() * 255.f;
                pixels[pixel + 1] = color.y() * 255.f;
                pixels[pixel + 2] = color.z() * 255.f;
            }

            inline void set_depth(int &x, int &y, const float &d)
            {
                depth[y * w + x] = d;
            }

            void clear()
            {
                int length = h * w;
                for (int y = 0; y < h; y++)
                {
                    for (int x = 0; x < w; x++)
                    {
                        unsigned char color = 0;//(y * w + x) * 255.f / length;
                        set_depth(x, y, -1000);
                        set_color(x, y, color, color, color);
                    }
                }
            }
        };
        int w;
        int h;
        const char* title =  "Light Bringer - Made By YBT";
        GLFWwindow *window;
        Camera *cam;
        Transformer *transformer;
        FrameBuffer *frame_buffer;
        std::vector<Mesh> world;
        std::vector<Light> lights;
        void processInput(double&& delta_time);
        void render(YYLB::Triangle &t);
        void render(std::vector<YYLB::Mesh> &ts);
        void vertext_shader();
        void pixel_shader();

    public:
        Render(int _w = 800, int _h = 600);
        void start();
    };
}
#endif