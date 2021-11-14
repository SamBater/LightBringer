#include <Core/Render.h>

namespace YYLB
{
    void Render::processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
        }
    }

    void Render::render(std::vector<YYLB::Triangle> &ts)
    {
        for (auto &t : ts)
        {
            render(t);
        }
    }

    void Render::render(YYLB::Triangle &t)
    {

        //求出三角形包围盒
        int topY = YYLB::max3(t[0].y(), t[1].y(), t[2].y());
        int botY = YYLB::min3(t[0].y(), t[1].y(), t[2].y());
        int rightX = YYLB::max3(t[0].x(), t[1].x(), t[2].x());
        int leftX = YYLB::min3(t[0].x(), t[1].x(), t[2].x());

#pragma omp parallel for num_threads(2)
        for (int y = botY; y < topY; y++)
            for (int x = leftX; x < rightX; x++)
            {

                //auto color = 0xff;
                int pixel = w * y * 3 + x * 3;
                unsigned char color = YYLB::Triangle::inside(x, y, t) ? 0xff : pixels[pixel];
                pixels[pixel] = color;
                pixels[pixel + 1] = color;
                pixels[pixel + 2] = color;
            }
    }

    void Render::start()
    {
        if (window == nullptr)
            return;

        YYLB::Vec2i a{100, 10}, b{40, 600}, c{300, 70};
        YYLB::Triangle t(a, b, c);

        YYLB::Vec2i a2{1000, 600}, b2{1000, 10}, c2{600, 10};
        YYLB::Triangle t2(a2, b2, c2);

        YYLB::Vec2i a3{w / 2, h / 2}, b3{w / 2, h}, c3{w, h / 2};
        YYLB::Triangle t3(a3, b3, c3);

        std::vector<YYLB::Triangle> ts;
        ts.push_back(t);
        ts.push_back(t2);
        ts.push_back(t3);

        auto start = std::chrono::high_resolution_clock::now();
        int cnt = 0;
        while (!glfwWindowShouldClose(window) && cnt < 120)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //processInput();
            render(ts);
            glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            glfwSwapBuffers(window);
            cnt++;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        printf("fps:%lf\n", std::chrono::duration<double>(cnt) / duration);
    }

    Render::Render(int _w, int _h) : w(_w), h(_h)
    {
        if (!glfwInit())
        {
            printf("Couldn't init GLFW\n");
            return;
        }

        window = glfwCreateWindow(w, h, "Light Bringer - Made By YBT", NULL, NULL);
        if (!window)
        {
            printf("Couldn't open window\n");
            return;
        }
        glfwMakeContextCurrent(window);
        pixels = new unsigned char[w * h * 3];
    }
}