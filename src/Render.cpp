#include <Core/Render.h>

namespace YYLB
{
    void Render::processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
        }
    }

    void Render::render(std::vector<YYLB::Mesh> &meshs)
    {
        for (auto &mesh : meshs)
        {
            auto world_pos = mesh.get_worldPos();
            auto triangles = mesh.get_triangles();

            for (auto &t : triangles)
            {

                Vec4f vertex_ss[3] =
                    {
                        tfmer->vertex_output(t.v3d_local[0], world_pos),
                        tfmer->vertex_output(t.v3d_local[1], world_pos),
                        tfmer->vertex_output(t.v3d_local[2], world_pos)};

                //设置三角形,准备光栅化
                t.ready_to_raser(vertex_ss);
                render(t);
            }
        }
    }

    template <typename T>
    T &clamp(T val, const T min, const T max)
    {
        if (val > max)
            val = max;
        if (val < min)
            val = min;
        return val;
    }

    void Render::render(YYLB::Triangle &t)
    {
        //包围盒AABB
        const YYLB::BoundingBox *aabb = t.bounding_box();
        int topY = aabb->top;
        int botY = aabb->bot;
        int rightX = aabb->right;
        int leftX = aabb->left;

        // #pragma omp parallel for num_threads(2)
        for (int y = botY; y < topY; y++)
            for (int x = leftX; x < rightX; x++)
            {
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

        //准备物体
        YYLB::Mesh m1(0, -4, -30);

        YYLB::Vec3f leftTop{10, 10, 10}, leftBot{10, -10, 10}, rt{-10, 10, 10}, rb{-10, -10, 10};
        auto ts = m1.get_triangles();
        m1.get_triangles().push_back(YYLB::Triangle(leftTop, leftBot, rt));
        m1.get_triangles().push_back(YYLB::Triangle(rt, rb, leftBot));

        std::vector<Mesh> world;
        world.push_back(std::move(m1));

        auto start = std::chrono::high_resolution_clock::now();
        int cnt = 0;
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //processInput();
            render(world);
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
        //设置opengl
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

        //设置渲染状态
        cam = new Camera(0, 1, 0, PI / 2, w * 1.f / h, 2.f, -400.f);
        tfmer = new Transformer();
        tfmer->set_world_to_view(cam->getPos());
        tfmer->set_view_to_project(cam);
        tfmer->set_projection_to_screen(w, h);
    }
}