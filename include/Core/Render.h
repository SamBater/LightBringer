#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GLFW/glfw3.h>
#include <math/Triangle.h>
#include <math/Common.h>
#include <math/Matrix.h>
#include <Core/Camera.h>
#include <math/Mesh.h>
#include <vector>
#include <chrono>
#include <Core/Transformer.h>
namespace YYLB
{
    class Render
    {
    private:
        int w;
        int h;

        GLFWwindow *window;
        Camera *cam;
        Transformer *tfmer;
        unsigned char *pixels;
        void processInput();
        void render(YYLB::Triangle &t);
        void render(std::vector<YYLB::Mesh> &ts);
        void vertext_shader();
        void pixel_shader();

    public:
        Render(int _w = 800, int _h = 600);
        void start();
        ~Render()
        {
            delete[] pixels;
        }
    };
}
